#include "case.h"
#include "ccv_case.h"
#include "ccv_nnc_case.h"
#include <ccv.h>
#include <nnc/ccv_nnc.h>
#include <nnc/ccv_nnc_easy.h>
#include <inc/ccv_convnet_internal.h>

TEST_SETUP()
{
	ccv_nnc_init();
}

static ccv_nnc_symbolic_graph_t* ccv_nnc_simple_symbolic_graph(ccv_convnet_t* convnet, ccv_nnc_tensor_t* input, ccv_nnc_tensor_t* output, ccv_nnc_graph_exec_symbol_t* source_symbol, ccv_nnc_graph_exec_symbol_t* dest_symbol, ccv_nnc_tensor_symbol_t* input_symbol_ref, ccv_nnc_tensor_symbol_t* output_symbol_ref, ccv_nnc_tensor_symbol_t* w_symbols, ccv_nnc_tensor_symbol_t* bias_symbols)
{
	int i;
	// We only create the graph compute to the last fc layer.
	ccv_nnc_symbolic_graph_t* symbolic_vgg = ccv_nnc_symbolic_graph_new();
	ccv_nnc_tensor_param_t input_info = input->info;
	ccv_nnc_tensor_symbol_t input_symbol = ccv_nnc_tensor_symbol_new(symbolic_vgg, input->info, 0);
	*input_symbol_ref = input_symbol;
	ccv_nnc_tensor_symbol_t output_symbol = ccv_nnc_tensor_symbol_new(symbolic_vgg, output->info, 0);
	*output_symbol_ref = output_symbol;
	ccv_nnc_graph_exec_symbol_t previous_exec_symbol;
	for (i = 0; i < convnet->count; i++)
	{
		ccv_convnet_layer_t* layer = convnet->layers + i;
		int rows, cols, partition;
		ccv_convnet_make_output(layer, layer->input.matrix.rows, layer->input.matrix.cols, &rows, &cols, &partition);
		ccv_nnc_tensor_param_t tensor_info = output->info;
		ccv_nnc_tensor_symbol_t tensor_symbol = output_symbol;
		if (i < convnet->count - 1)
		{
			if (layer->type == CCV_CONVNET_FULL_CONNECT)
				tensor_info = CPU_TENSOR_NHWC(32F, rows * cols * partition);
			else
				tensor_info = CPU_TENSOR_NHWC(32F, rows, cols, (layer->type == CCV_CONVNET_CONVOLUTIONAL ? layer->net.convolutional.count : layer->input.matrix.channels));
			tensor_symbol = ccv_nnc_tensor_symbol_new(symbolic_vgg, tensor_info, 0);
		}
		ccv_nnc_graph_exec_symbol_t exec_symbol = {0};
		if (layer->type == CCV_CONVNET_CONVOLUTIONAL)
		{
			ccv_nnc_tensor_symbol_t w_symbol = ccv_nnc_tensor_symbol_new(symbolic_vgg, CPU_TENSOR_NHWC(32F, layer->net.convolutional.count, layer->net.convolutional.rows, layer->net.convolutional.cols, layer->net.convolutional.channels), 0);
			w_symbols[i] = w_symbol;
			ccv_nnc_tensor_symbol_t bias_symbol = ccv_nnc_tensor_symbol_new(symbolic_vgg, CPU_TENSOR_NHWC(32F, layer->net.convolutional.count), 0);
			bias_symbols[i] = bias_symbol;
			ccv_nnc_cmd_t cmd = CMD_CONVOLUTION_FORWARD(1, layer->net.convolutional.count, layer->net.convolutional.rows, layer->net.convolutional.cols, layer->net.convolutional.channels);
			exec_symbol = ccv_nnc_graph_exec_symbol_new(symbolic_vgg, cmd, TENSOR_SYMBOL_LIST(input_symbol, w_symbol, bias_symbol), TENSOR_SYMBOL_LIST(tensor_symbol), 0);
		} else if (layer->type == CCV_CONVNET_MAX_POOL) {
			ccv_nnc_cmd_t cmd = CMD_MAX_POOL_FORWARD(layer->net.pool.size, layer->net.pool.size);
			exec_symbol = ccv_nnc_graph_exec_symbol_new(symbolic_vgg, cmd, TENSOR_SYMBOL_LIST(input_symbol), TENSOR_SYMBOL_LIST(tensor_symbol), 0);
		} else if (layer->type == CCV_CONVNET_FULL_CONNECT) {
			ccv_nnc_tensor_symbol_t w_symbol = ccv_nnc_tensor_symbol_new(symbolic_vgg, CPU_TENSOR_NHWC(32F, layer->net.full_connect.count, layer->input.node.count), 0);
			w_symbols[i] = w_symbol;
			ccv_nnc_tensor_symbol_t bias_symbol = ccv_nnc_tensor_symbol_new(symbolic_vgg, CPU_TENSOR_NHWC(32F, layer->net.full_connect.count), 0);
			bias_symbols[i] = bias_symbol;
			ccv_nnc_cmd_t cmd = CMD_GEMM_FORWARD(NO_TRANSPOSE, TRANSPOSE(0, 1));
			// If the input is not what I expected (array), reshape it.
			if (input_info.dim[0] != ccv_nnc_tensor_count(input_info))
				input_symbol = ccv_nnc_tensor_symbol_alias_new(symbolic_vgg, input_symbol, ccv_nnc_no_ofs, DIM_ALLOC(1), CPU_TENSOR_NHWC(32F, ccv_nnc_tensor_count(input_info)), 0);
			exec_symbol = ccv_nnc_graph_exec_symbol_new(symbolic_vgg, cmd, TENSOR_SYMBOL_LIST(input_symbol, w_symbol, bias_symbol), TENSOR_SYMBOL_LIST(tensor_symbol), 0);
		} else {
			assert("unreachable");
		}
		if (i != 0)
			ccv_nnc_graph_exec_symbol_concat(symbolic_vgg, previous_exec_symbol, exec_symbol);
		previous_exec_symbol = exec_symbol;
		if (i == 0)
			*source_symbol = exec_symbol;
		if (i < convnet->count - 1 &&
			(layer->type == CCV_CONVNET_CONVOLUTIONAL || layer->type == CCV_CONVNET_FULL_CONNECT))
		{
			// Create the ReLU layer.
			ccv_nnc_cmd_t cmd = CMD_RELU_FORWARD();
			ccv_nnc_tensor_symbol_t next_symbol = ccv_nnc_tensor_symbol_new(symbolic_vgg, tensor_info, 0);
			exec_symbol = ccv_nnc_graph_exec_symbol_new(symbolic_vgg, cmd, TENSOR_SYMBOL_LIST(tensor_symbol), TENSOR_SYMBOL_LIST(next_symbol), 0);
			ccv_nnc_graph_exec_symbol_concat(symbolic_vgg, previous_exec_symbol, exec_symbol);
			tensor_symbol = next_symbol;
			previous_exec_symbol = exec_symbol;
		}
		if (i == convnet->count - 1)
			*dest_symbol = exec_symbol;
		// This is the input of next layer.
		input_symbol = tensor_symbol;
		input_info = tensor_info;
	}
	return symbolic_vgg;
}

#ifdef HAVE_LIBPNG
TEST_CASE("run vgg-d graph from its symbolic representation")
{
	ccv_convnet_t* convnet = ccv_convnet_read(0, "../../../samples/image-net-2012-vgg-d.sqlite3");
	ccv_dense_matrix_t* image = 0;
	ccv_read("../../../samples/dex.png", &image, CCV_IO_ANY_FILE | CCV_IO_RGB_COLOR);
	ccv_dense_matrix_t* input = 0;
	ccv_convnet_input_formation(convnet->input, image, &input);
	ccv_matrix_free(image);
	ccv_dense_matrix_t* sliced = 0;
	ccv_slice(input, (ccv_matrix_t**)&sliced, 0, (input->rows - 225) / 2, (input->cols - 225) / 2, 225, 225);
	ccv_matrix_free(input);
	ccv_dense_matrix_t* b = 0;
	ccv_convnet_encode(convnet, &sliced, &b, 1);
	ccv_nnc_tensor_t* c = ccv_nnc_tensor_new(0, CPU_TENSOR_NHWC(32F, 1000), 0);
	ccv_nnc_tensor_symbol_t* w_symbols = ccmalloc(sizeof(ccv_nnc_tensor_symbol_t) * convnet->count);
	ccv_nnc_tensor_symbol_t* bias_symbols = ccmalloc(sizeof(ccv_nnc_tensor_symbol_t) * convnet->count);
	ccv_nnc_graph_exec_symbol_t source_symbol, dest_symbol;
	ccv_nnc_tensor_symbol_t input_symbol, output_symbol;
	ccv_nnc_symbolic_graph_t* graph = ccv_nnc_simple_symbolic_graph(convnet, (ccv_nnc_tensor_t*)sliced, c, &source_symbol, &dest_symbol, &input_symbol, &output_symbol, w_symbols, bias_symbols);
	ccv_nnc_graph_t* run_graph = 0;
	ccv_nnc_tensor_arena_t* tensor_arena = 0;
	ccv_nnc_graph_exec_arena_t* graph_exec_arena = 0;
	SYMBOLIC_GRAPH_GEN(graph, CCV_NNC_LONG_DOT_GRAPH);
	ccv_nnc_symbolic_graph_compile(graph, ccv_nnc_default_compile_params, TENSOR_BIND_MAP(KV(input_symbol, (ccv_nnc_tensor_t*)sliced), KV(output_symbol, c)), 0, 0, GRAPH_EXEC_SYMBOL_LIST(source_symbol), GRAPH_EXEC_SYMBOL_LIST(dest_symbol), &run_graph, &tensor_arena, &graph_exec_arena);
	GRAPH_GEN(run_graph, CCV_NNC_LONG_DOT_GRAPH);
	REQUIRE(ccv_nnc_tensor_arena_size(tensor_arena) <= 471513504, "the allocated size should be smaller than what we previously got");
	int i;
	for (i = 0; i < convnet->count; i++)
	{
		ccv_convnet_layer_t* layer = convnet->layers + i;
		if (layer->type == CCV_CONVNET_CONVOLUTIONAL)
		{
			ccv_nnc_tensor_t* w = ccv_nnc_tensor_from_symbol(tensor_arena, w_symbols[i]);
			memcpy(w->data.f32, layer->w, layer->wnum * sizeof(float));
			ccv_nnc_tensor_t* bias = ccv_nnc_tensor_from_symbol(tensor_arena, bias_symbols[i]);
			memcpy(bias->data.f32, layer->bias, layer->net.convolutional.count * sizeof(float));
		} else if (layer->type == CCV_CONVNET_FULL_CONNECT) {
			ccv_nnc_tensor_t* w = ccv_nnc_tensor_from_symbol(tensor_arena, w_symbols[i]);
			memcpy(w->data.f32, layer->w, layer->wnum * sizeof(float));
			ccv_nnc_tensor_t* bias = ccv_nnc_tensor_from_symbol(tensor_arena, bias_symbols[i]);
			memcpy(bias->data.f32, layer->bias, layer->net.full_connect.count * sizeof(float));
		}
	}
	ccv_nnc_graph_autotune(run_graph, 0, 0, GRAPH_EXEC_LIST(ccv_nnc_graph_exec_from_symbol(graph_exec_arena, source_symbol)), GRAPH_EXEC_LIST(ccv_nnc_graph_exec_from_symbol(graph_exec_arena, dest_symbol)));
	// Repopulate the weight. Since the weight tensor may be reused, need to repopulate every time run.
	// To avoid this, bind the weights directly or declare as const.
	for (i = 0; i < convnet->count; i++)
	{
		ccv_convnet_layer_t* layer = convnet->layers + i;
		if (layer->type == CCV_CONVNET_CONVOLUTIONAL)
		{
			ccv_nnc_tensor_t* w = ccv_nnc_tensor_from_symbol(tensor_arena, w_symbols[i]);
			memcpy(w->data.f32, layer->w, layer->wnum * sizeof(float));
			ccv_nnc_tensor_t* bias = ccv_nnc_tensor_from_symbol(tensor_arena, bias_symbols[i]);
			memcpy(bias->data.f32, layer->bias, layer->net.convolutional.count * sizeof(float));
		} else if (layer->type == CCV_CONVNET_FULL_CONNECT) {
			ccv_nnc_tensor_t* w = ccv_nnc_tensor_from_symbol(tensor_arena, w_symbols[i]);
			memcpy(w->data.f32, layer->w, layer->wnum * sizeof(float));
			ccv_nnc_tensor_t* bias = ccv_nnc_tensor_from_symbol(tensor_arena, bias_symbols[i]);
			memcpy(bias->data.f32, layer->bias, layer->net.full_connect.count * sizeof(float));
		}
	}
	ccv_nnc_graph_run(run_graph, 0, GRAPH_EXEC_LIST(ccv_nnc_graph_exec_from_symbol(graph_exec_arena, source_symbol)), GRAPH_EXEC_LIST(ccv_nnc_graph_exec_from_symbol(graph_exec_arena, dest_symbol)), 0, 0);
	REQUIRE_ARRAY_EQ_WITH_TOLERANCE(float, b->data.f32, c->data.f32, 1000, 1e-4, "output should be the same from convnet and from the symbolic graph");
	ccv_nnc_tensor_free(c);
	ccv_matrix_free(sliced);
	ccv_matrix_free(b);
	ccv_nnc_symbolic_graph_free(graph);
	ccv_nnc_graph_free(run_graph);
	ccv_nnc_tensor_arena_free(tensor_arena);
	ccv_nnc_graph_exec_arena_free(graph_exec_arena);
	ccfree(w_symbols);
	ccfree(bias_symbols);
	ccv_convnet_free(convnet);
}
#endif

#include "case_main.h"
