#include "ccv.h"
#include "case.h"
#include "ccv_case.h"

TEST_CASE("read raw memory, rgb => gray")
{
	unsigned char rgb[] = {
		10, 20, 30, 40, 50, 60, 70,
		80, 90, 15, 25, 35, 45, 55,
	};
	ccv_dense_matrix_t* x = 0;
	ccv_read(rgb, &x, CCV_IO_RGB_RAW | CCV_IO_GRAY, 2, 2, 7);
	unsigned char hx1[] = {
		18, 48,
	};
	REQUIRE_ARRAY_EQ(unsigned char, hx1, x->data.u8, 2, "1st row when reading raw rgb ordered memory block into grayscale matrix doesn't match");
	unsigned char hx2[] = {
		82, 33,
	};
	REQUIRE_ARRAY_EQ(unsigned char, hx2, x->data.u8 + x->step, 2, "2nd row when reading raw rgb ordered memory block into grayscale matrix doesn't match");
	ccv_matrix_free(x);
}

TEST_CASE("read raw memory, rgb => rgb")
{
	unsigned char rgb[] = {
		10, 20, 30, 40, 50, 60, 70,
		80, 90, 15, 25, 35, 45, 55,
	};
	ccv_dense_matrix_t* x = 0;
	ccv_read(rgb, &x, CCV_IO_RGB_RAW | CCV_IO_RGB_COLOR, 2, 2, 7);
	REQUIRE_ARRAY_EQ(unsigned char, rgb, x->data.u8, 6, "1st row when reading raw rgb ordered memory block into rgb matrix doesn't match");
	REQUIRE_ARRAY_EQ(unsigned char, rgb + 7, x->data.u8 + x->step, 6, "2nd row when reading raw rgb ordered memory block into rgb matrix doesn't match");
	ccv_matrix_free(x);
}

TEST_CASE("read raw memory, rgba => rgb")
{
	unsigned char rgba[] = {
		10, 20, 30, 40, 50, 60, 70, 80, 90,
		15, 25, 35, 45, 55, 65, 75, 85, 95,
	};
	ccv_dense_matrix_t* x = 0;
	ccv_read(rgba, &x, CCV_IO_RGBA_RAW | CCV_IO_RGB_COLOR, 2, 2, 9);
	unsigned char hx1[] = {
		10, 20, 30, 50, 60, 70,
	};
	REQUIRE_ARRAY_EQ(unsigned char, hx1, x->data.u8, 6, "1st row when reading raw rgba memory block into rgb matrix doesn't match");
	unsigned char hx2[] = {
		15, 25, 35, 55, 65, 75,
	};
	REQUIRE_ARRAY_EQ(unsigned char, hx2, x->data.u8 + x->step, 6, "2nd row when reading raw rgba memory block into rgb matrix doesn't match");
	ccv_matrix_free(x);
}

TEST_CASE("read raw memory, rgba => gray")
{
	unsigned char rgba[] = {
		10, 20, 30, 40, 50, 60, 70, 80, 90,
		15, 25, 35, 45, 55, 65, 75, 85, 95,
	};
	ccv_dense_matrix_t* x = 0;
	ccv_read(rgba, &x, CCV_IO_RGBA_RAW | CCV_IO_GRAY, 2, 2, 9);
	unsigned char hx1[] = {
		18, 58,
	};
	REQUIRE_ARRAY_EQ(unsigned char, hx1, x->data.u8, 2, "1st row when reading raw rgba memory block into grayscale matrix doesn't match");
	unsigned char hx2[] = {
		23, 63,
	};
	REQUIRE_ARRAY_EQ(unsigned char, hx2, x->data.u8 + x->step, 2, "2nd row when reading raw rgba memory block into grayscale matrix doesn't match");
	ccv_matrix_free(x);
}

TEST_CASE("read raw memory, rgba => rgba")
{
	unsigned char rgba[] = {
		10, 20, 30, 40, 50, 60, 70, 80, 90,
		15, 25, 35, 45, 55, 65, 75, 85, 95,
	};
	ccv_dense_matrix_t* x = 0;
	ccv_read(rgba, &x, CCV_IO_RGBA_RAW, 2, 2, 9);
	REQUIRE_ARRAY_EQ(unsigned char, rgba, x->data.u8, 8, "1st row when reading raw rgba memory block into rgba matrix doesn't match");
	REQUIRE_ARRAY_EQ(unsigned char, rgba + 9, x->data.u8 + x->step, 8, "2nd row when reading raw rgba memory block into rgba matrix doesn't match");
	ccv_matrix_free(x);
}

TEST_CASE("read raw memory, argb => rgb")
{
	unsigned char argb[] = {
		10, 20, 30, 40, 50, 60, 70, 80, 90,
		15, 25, 35, 45, 55, 65, 75, 85, 95,
	};
	ccv_dense_matrix_t* x = 0;
	ccv_read(argb, &x, CCV_IO_ARGB_RAW | CCV_IO_RGB_COLOR, 2, 2, 9);
	unsigned char hx1[] = {
		20, 30, 40, 60, 70, 80,
	};
	REQUIRE_ARRAY_EQ(unsigned char, hx1, x->data.u8, 6, "1st row when reading raw argb memory block into rgb matrix doesn't match");
	unsigned char hx2[] = {
		25, 35, 45, 65, 75, 85,
	};
	REQUIRE_ARRAY_EQ(unsigned char, hx2, x->data.u8 + x->step, 6, "2nd row when reading raw argb memory block into rgb matrix doesn't match");
	ccv_matrix_free(x);
}

TEST_CASE("read raw memory, argb => gray")
{
	unsigned char argb[] = {
		10, 20, 30, 40, 50, 60, 70, 80, 90,
		15, 25, 35, 45, 55, 65, 75, 85, 95,
	};
	ccv_dense_matrix_t* x = 0;
	ccv_read(argb, &x, CCV_IO_ARGB_RAW | CCV_IO_GRAY, 2, 2, 9);
	unsigned char hx1[] = {
		28, 68,
	};
	REQUIRE_ARRAY_EQ(unsigned char, hx1, x->data.u8, 2, "1st row when reading raw argb memory block into grayscale matrix doesn't match");
	unsigned char hx2[] = {
		33, 73,
	};
	REQUIRE_ARRAY_EQ(unsigned char, hx2, x->data.u8 + x->step, 2, "2nd row when reading raw argb memory block into grayscale matrix doesn't match");
	ccv_matrix_free(x);
}

TEST_CASE("read raw memory, argb => argb")
{
	unsigned char argb[] = {
		10, 20, 30, 40, 50, 60, 70, 80, 90,
		15, 25, 35, 45, 55, 65, 75, 85, 95,
	};
	ccv_dense_matrix_t* x = 0;
	ccv_read(argb, &x, CCV_IO_ARGB_RAW, 2, 2, 9);
	REQUIRE_ARRAY_EQ(unsigned char, argb, x->data.u8, 8, "1st row when reading raw argb memory block into argb matrix doesn't match");
	REQUIRE_ARRAY_EQ(unsigned char, argb + 9, x->data.u8 + x->step, 8, "2nd row when reading raw argb memory block into argb matrix doesn't match");
	ccv_matrix_free(x);
}

TEST_CASE("read raw memory, bgr => gray")
{
	unsigned char bgr[] = {
		10, 20, 30, 40, 50, 60, 70,
		80, 90, 15, 25, 35, 45, 55,
	};
	ccv_dense_matrix_t* x = 0;
	ccv_read(bgr, &x, CCV_IO_BGR_RAW | CCV_IO_GRAY, 2, 2, 7);
	unsigned char hx1[] = {
		21, 51,
	};
	REQUIRE_ARRAY_EQ(unsigned char, hx1, x->data.u8, 2, "1st row when reading raw bgr ordered memory block into grayscale matrix doesn't match");
	unsigned char hx2[] = {
		73, 36,
	};
	REQUIRE_ARRAY_EQ(unsigned char, hx2, x->data.u8 + x->step, 2, "2nd row when reading raw bgr ordered memory block into grayscale matrix doesn't match");
	ccv_matrix_free(x);
}

TEST_CASE("read raw memory, bgr => rgb")
{
	unsigned char bgr[] = {
		10, 20, 30, 40, 50, 60, 70,
		80, 90, 15, 25, 35, 45, 55,
	};
	ccv_dense_matrix_t* x = 0;
	ccv_read(bgr, &x, CCV_IO_BGR_RAW | CCV_IO_RGB_COLOR, 2, 2, 7);
	unsigned char hx1[] = {
		30, 20, 10, 60, 50, 40,
	};
	REQUIRE_ARRAY_EQ(unsigned char, hx1, x->data.u8, 6, "1st row when reading raw bgr ordered memory block into rgb matrix doesn't match");
	unsigned char hx2[] = {
		15, 90, 80, 45, 35, 25,
	};
	REQUIRE_ARRAY_EQ(unsigned char, hx2, x->data.u8 + x->step, 6, "2nd row when reading raw bgr ordered memory block into rgb matrix doesn't match");
	ccv_matrix_free(x);
}

TEST_CASE("read raw memory, bgr => bgr")
{
	unsigned char bgr[] = {
		10, 20, 30, 40, 50, 60, 70,
		80, 90, 15, 25, 35, 45, 55,
	};
	ccv_dense_matrix_t* x = 0;
	ccv_read(bgr, &x, CCV_IO_BGR_RAW, 2, 2, 7);
	REQUIRE_ARRAY_EQ(unsigned char, bgr, x->data.u8, 6, "1st row when reading raw bgr ordered memory block into rgb matrix doesn't match");
	REQUIRE_ARRAY_EQ(unsigned char, bgr + 7, x->data.u8 + x->step, 6, "2nd row when reading raw bgr ordered memory block into rgb matrix doesn't match");
	ccv_matrix_free(x);
}

TEST_CASE("read raw memory, bgra => rgb")
{
	unsigned char bgra[] = {
		10, 20, 30, 40, 50, 60, 70, 80, 90,
		15, 25, 35, 45, 55, 65, 75, 85, 95,
	};
	ccv_dense_matrix_t* x = 0;
	ccv_read(bgra, &x, CCV_IO_BGRA_RAW | CCV_IO_RGB_COLOR, 2, 2, 9);
	unsigned char hx1[] = {
		30, 20, 10, 70, 60, 50,
	};
	REQUIRE_ARRAY_EQ(unsigned char, hx1, x->data.u8, 6, "1st row when reading raw bgra memory block into rgb matrix doesn't match");
	unsigned char hx2[] = {
		35, 25, 15, 75, 65, 55,
	};
	REQUIRE_ARRAY_EQ(unsigned char, hx2, x->data.u8 + x->step, 6, "2nd row when reading raw bgra memory block into rgb matrix doesn't match");
	ccv_matrix_free(x);
}

TEST_CASE("read raw memory, bgra => gray")
{
	unsigned char bgra[] = {
		10, 20, 30, 40, 50, 60, 70, 80, 90,
		15, 25, 35, 45, 55, 65, 75, 85, 95,
	};
	ccv_dense_matrix_t* x = 0;
	ccv_read(bgra, &x, CCV_IO_BGRA_RAW | CCV_IO_GRAY, 2, 2, 9);
	unsigned char hx1[] = {
		21, 61,
	};
	REQUIRE_ARRAY_EQ(unsigned char, hx1, x->data.u8, 2, "1st row when reading raw bgra memory block into grayscale matrix doesn't match");
	unsigned char hx2[] = {
		26, 66,
	};
	REQUIRE_ARRAY_EQ(unsigned char, hx2, x->data.u8 + x->step, 2, "2nd row when reading raw bgra memory block into grayscale matrix doesn't match");
	ccv_matrix_free(x);
}

TEST_CASE("read raw memory, bgra => bgra")
{
	unsigned char bgra[] = {
		10, 20, 30, 40, 50, 60, 70, 80, 90,
		15, 25, 35, 45, 55, 65, 75, 85, 95,
	};
	ccv_dense_matrix_t* x = 0;
	ccv_read(bgra, &x, CCV_IO_BGRA_RAW, 2, 2, 9);
	REQUIRE_ARRAY_EQ(unsigned char, bgra, x->data.u8, 8, "1st row when reading raw bgra memory block into bgra matrix doesn't match");
	REQUIRE_ARRAY_EQ(unsigned char, bgra + 9, x->data.u8 + x->step, 8, "2nd row when reading raw bgra memory block into bgra matrix doesn't match");
	ccv_matrix_free(x);
}

TEST_CASE("read raw memory, abgr => rgb")
{
	unsigned char abgr[] = {
		10, 20, 30, 40, 50, 60, 70, 80, 90,
		15, 25, 35, 45, 55, 65, 75, 85, 95,
	};
	ccv_dense_matrix_t* x = 0;
	ccv_read(abgr, &x, CCV_IO_ABGR_RAW | CCV_IO_RGB_COLOR, 2, 2, 9);
	unsigned char hx1[] = {
		40, 30, 20, 80, 70, 60,
	};
	REQUIRE_ARRAY_EQ(unsigned char, hx1, x->data.u8, 6, "1st row when reading raw abgr memory block into rgb matrix doesn't match");
	unsigned char hx2[] = {
		45, 35, 25, 85, 75, 65,
	};
	REQUIRE_ARRAY_EQ(unsigned char, hx2, x->data.u8 + x->step, 6, "2nd row when reading raw abgr memory block into rgb matrix doesn't match");
	ccv_matrix_free(x);
}

TEST_CASE("read raw memory, abgr => gray")
{
	unsigned char abgr[] = {
		10, 20, 30, 40, 50, 60, 70, 80, 90,
		15, 25, 35, 45, 55, 65, 75, 85, 95,
	};
	ccv_dense_matrix_t* x = 0;
	ccv_read(abgr, &x, CCV_IO_ABGR_RAW | CCV_IO_GRAY, 2, 2, 9);
	unsigned char hx1[] = {
		31, 71,
	};
	REQUIRE_ARRAY_EQ(unsigned char, hx1, x->data.u8, 2, "1st row when reading raw abgr memory block into grayscale matrix doesn't match");
	unsigned char hx2[] = {
		36, 76,
	};
	REQUIRE_ARRAY_EQ(unsigned char, hx2, x->data.u8 + x->step, 2, "2nd row when reading raw abgr memory block into grayscale matrix doesn't match");
	ccv_matrix_free(x);
}

TEST_CASE("read raw memory, abgr => abgr")
{
	unsigned char abgr[] = {
		10, 20, 30, 40, 50, 60, 70, 80, 90,
		15, 25, 35, 45, 55, 65, 75, 85, 95,
	};
	ccv_dense_matrix_t* x = 0;
	ccv_read(abgr, &x, CCV_IO_ABGR_RAW, 2, 2, 9);
	REQUIRE_ARRAY_EQ(unsigned char, abgr, x->data.u8, 8, "1st row when reading raw abgr memory block into abgr matrix doesn't match");
	REQUIRE_ARRAY_EQ(unsigned char, abgr + 9, x->data.u8 + x->step, 8, "2nd row when reading raw abgr memory block into abgr matrix doesn't match");
	ccv_matrix_free(x);
}

TEST_CASE("read raw memory, gray => rgb")
{
	unsigned char g[] = {
		10, 20, 30, 40, 50,
		15, 25, 35, 45, 55,
	};
	ccv_dense_matrix_t* x = 0;
	ccv_read(g, &x, CCV_IO_GRAY_RAW | CCV_IO_RGB_COLOR, 2, 2, 5);
	unsigned char hx1[] = {
		10, 10, 10, 20, 20, 20,
	};
	REQUIRE_ARRAY_EQ(unsigned char, hx1, x->data.u8, 6, "1st row when reading raw grayscale memory block into rgb matrix doesn't match");
	unsigned char hx2[] = {
		15, 15, 15, 25, 25, 25,
	};
	REQUIRE_ARRAY_EQ(unsigned char, hx2, x->data.u8 + x->step, 6, "2nd row when reading raw grayscale memory block into rgb matrix doesn't match");
	ccv_matrix_free(x);
}

TEST_CASE("read raw memory, gray => gray")
{
	unsigned char g[] = {
		10, 20, 30, 40, 50,
		15, 25, 35, 45, 55,
	};
	ccv_dense_matrix_t* x = 0;
	ccv_read(g, &x, CCV_IO_GRAY_RAW | CCV_IO_GRAY, 2, 2, 5);
	REQUIRE_ARRAY_EQ(unsigned char, g, x->data.u8, 2, "1st row when reading raw grayscale memory block into grayscale matrix doesn't match");
	REQUIRE_ARRAY_EQ(unsigned char, g + 5, x->data.u8 + x->step, 2, "2nd row when reading raw grayscale memory block into grayscale matrix doesn't match");
	ccv_matrix_free(x);
}

TEST_CASE("read raw memory with no copy mode")
{
	unsigned char rgb[] = {
		10, 20, 30, 40, 50, 60, 70, 80, 90,
		15, 25, 35, 45, 55, 65, 75, 85, 95,
	};
	ccv_dense_matrix_t* x = 0;
	ccv_read(rgb, &x, CCV_IO_ANY_RAW | CCV_IO_NO_COPY, 2, 2, 9);
	REQUIRE_EQ(9, x->step, "its step value should be equal to the passing scanline value in no copy mode");
	REQUIRE(rgb == x->data.u8, "its data section should point to the same memory region");
	ccv_matrix_free(x);
}

#ifdef HAVE_LIBJPEG
TEST_CASE("read JPEG from memory")
{
	ccv_dense_matrix_t* x = 0;
	ccv_read("../../samples/cmyk-jpeg-format.jpg", &x, CCV_IO_ANY_FILE);
	ccv_dense_matrix_t* y = 0;
	FILE* rb = fopen("../../samples/cmyk-jpeg-format.jpg", "rb");
	fseek(rb, 0, SEEK_END);
	long size = ftell(rb);
	char* data = (char*)ccmalloc(size);
	fseek(rb, 0, SEEK_SET);
	fread(data, 1, size, rb);
	fclose(rb);
	ccv_read(data, &y, CCV_IO_ANY_STREAM, size);
	ccfree(data);
	REQUIRE_MATRIX_EQ(x, y, "read cmyk-jpeg-format.jpg from file system and memory should be the same");
	ccv_matrix_free(y);
	ccv_matrix_free(x);
}
#endif

#ifdef HAVE_LIBPNG
TEST_CASE("read PNG from memory")
{
	ccv_dense_matrix_t* x = 0;
	ccv_read("../../samples/nature.png", &x, CCV_IO_ANY_FILE);
	ccv_dense_matrix_t* y = 0;
	FILE* rb = fopen("../../samples/nature.png", "rb");
	fseek(rb, 0, SEEK_END);
	long size = ftell(rb);
	char* data = (char*)ccmalloc(size);
	fseek(rb, 0, SEEK_SET);
	fread(data, 1, size, rb);
	fclose(rb);
	ccv_read(data, &y, CCV_IO_ANY_STREAM, size);
	ccfree(data);
	REQUIRE_MATRIX_EQ(x, y, "read nature.png from file system and memory should be the same");
	ccv_matrix_free(y);
	ccv_matrix_free(x);
}
#endif

#ifdef HAVE_LIBJPEG
TEST_CASE("write JPEG to memory")
{
	char sanitized_test_case_name[1024] = "/tmp/";
	strncpy(sanitized_test_case_name + 5, __case_name__, 1024 - 5);
	int i;
	for (i = 10; i < 1024 && sanitized_test_case_name[i]; i++)
		// If not A-Za-z0-9, replace with _
		if (!((sanitized_test_case_name[i] >= 'A' && sanitized_test_case_name[i] <= 'Z') ||
			 (sanitized_test_case_name[i] >= 'a' && sanitized_test_case_name[i] <= 'z') ||
			 (sanitized_test_case_name[i] >= '0' && sanitized_test_case_name[i] <= '9')))
			sanitized_test_case_name[i] = '_';
	assert(i < 1024);
	sanitized_test_case_name[i] = '.';
	sanitized_test_case_name[i + 1] = 'j';
	sanitized_test_case_name[i + 2] = 'p';
	sanitized_test_case_name[i + 3] = 'g';
	sanitized_test_case_name[i + 4] = 0;
	ccv_dense_matrix_t* x = 0;
	ccv_read("../../samples/nature.png", &x, CCV_IO_ANY_FILE);
	char* data = (char*)ccmalloc(x->rows * x->cols * 2);
	size_t len = x->rows * x->cols * 2;
	ccv_write(x, data, &len, CCV_IO_JPEG_STREAM, 0);
	ccv_write(x, sanitized_test_case_name, 0, CCV_IO_JPEG_FILE, 0);
	assert(len < x->rows * x->cols * 2);
	ccv_dense_matrix_t* y = 0;
	ccv_read(data, &y, CCV_IO_ANY_STREAM, len);
	ccfree(data);
	ccv_dense_matrix_t* z = 0;
	ccv_read(sanitized_test_case_name, &z, CCV_IO_ANY_FILE, 0);
	FILE* fd = fopen(sanitized_test_case_name, "rb");
	fseek(fd, 0L, SEEK_END);
	size_t fsz = ftell(fd);
	fclose(fd);
	REQUIRE_EQ(fsz, len, "size of both memory and file should be the same");
	REQUIRE_MATRIX_EQ(y, z, "write nature.png to JPEG in file system and memory should be the same");
	ccv_matrix_free(z);
	ccv_matrix_free(y);
	ccv_matrix_free(x);
}

TEST_CASE("write JPEG to memory fail at small memory allocation")
{
	ccv_dense_matrix_t* x = 0;
	ccv_read("../../samples/nature.png", &x, CCV_IO_ANY_FILE);
	char* data = (char*)ccmalloc(100);
	size_t len = 100;
	int error = ccv_write(x, data, &len, CCV_IO_JPEG_STREAM, 0);
	REQUIRE_EQ(error, CCV_IO_ERROR, "fail at write the file");
	ccfree(data);
	ccv_matrix_free(x);
}
#endif

#ifdef HAVE_LIBPNG
TEST_CASE("write PNG to memory")
{
	char sanitized_test_case_name[1024] = "/tmp/";
	strncpy(sanitized_test_case_name + 5, __case_name__, 1024 - 5);
	int i;
	for (i = 10; i < 1024 && sanitized_test_case_name[i]; i++)
		// If not A-Za-z0-9, replace with _
		if (!((sanitized_test_case_name[i] >= 'A' && sanitized_test_case_name[i] <= 'Z') ||
			 (sanitized_test_case_name[i] >= 'a' && sanitized_test_case_name[i] <= 'z') ||
			 (sanitized_test_case_name[i] >= '0' && sanitized_test_case_name[i] <= '9')))
			sanitized_test_case_name[i] = '_';
	assert(i < 1024);
	sanitized_test_case_name[i] = '.';
	sanitized_test_case_name[i + 1] = 'p';
	sanitized_test_case_name[i + 2] = 'n';
	sanitized_test_case_name[i + 3] = 'g';
	sanitized_test_case_name[i + 4] = 0;
	ccv_dense_matrix_t* x = 0;
	ccv_read("../../samples/nature.png", &x, CCV_IO_ANY_FILE);
	char* data = (char*)ccmalloc(x->rows * x->cols * 2);
	size_t len = x->rows * x->cols * 2;
	ccv_write(x, data, &len, CCV_IO_PNG_STREAM, 0);
	ccv_write(x, sanitized_test_case_name, 0, CCV_IO_PNG_FILE, 0);
	assert(len < x->rows * x->cols * 2);
	ccv_dense_matrix_t* y = 0;
	ccv_read(data, &y, CCV_IO_ANY_STREAM, len);
	ccfree(data);
	ccv_dense_matrix_t* z = 0;
	ccv_read(sanitized_test_case_name, &z, CCV_IO_ANY_FILE, 0);
	FILE* fd = fopen(sanitized_test_case_name, "rb");
	fseek(fd, 0L, SEEK_END);
	size_t fsz = ftell(fd);
	fclose(fd);
	REQUIRE_EQ(fsz, len, "size of both memory and file should be the same");
	REQUIRE_MATRIX_EQ(y, z, "write nature.png to PNG in file system and memory should be the same");
	ccv_matrix_free(z);
	ccv_matrix_free(y);
	ccv_matrix_free(x);
}

TEST_CASE("write PNG to memory fail at small memory allocation")
{
	ccv_dense_matrix_t* x = 0;
	ccv_read("../../samples/nature.png", &x, CCV_IO_ANY_FILE);
	char* data = (char*)ccmalloc(100);
	size_t len = 100;
	int error = ccv_write(x, data, &len, CCV_IO_PNG_STREAM, 0);
	REQUIRE_EQ(error, CCV_IO_ERROR, "fail at write the file");
	ccfree(data);
	ccv_matrix_free(x);
}
#endif

#ifdef HAVE_LIBPNG
TEST_CASE("write binary to memory")
{
	char sanitized_test_case_name[1024] = "/tmp/";
	strncpy(sanitized_test_case_name + 5, __case_name__, 1024 - 5);
	int i;
	for (i = 10; i < 1024 && sanitized_test_case_name[i]; i++)
		// If not A-Za-z0-9, replace with _
		if (!((sanitized_test_case_name[i] >= 'A' && sanitized_test_case_name[i] <= 'Z') ||
			 (sanitized_test_case_name[i] >= 'a' && sanitized_test_case_name[i] <= 'z') ||
			 (sanitized_test_case_name[i] >= '0' && sanitized_test_case_name[i] <= '9')))
			sanitized_test_case_name[i] = '_';
	assert(i < 1024);
	sanitized_test_case_name[i] = '.';
	sanitized_test_case_name[i + 1] = 'b';
	sanitized_test_case_name[i + 2] = 'i';
	sanitized_test_case_name[i + 3] = 'n';
	sanitized_test_case_name[i + 4] = 0;
	ccv_dense_matrix_t* x = 0;
	ccv_read("../../samples/nature.png", &x, CCV_IO_ANY_FILE);
	char* data = (char*)ccmalloc(x->rows * x->cols * 4);
	size_t len = x->rows * x->cols * 4;
	ccv_write(x, data, &len, CCV_IO_PLAIN_STREAM, 0);
	ccv_write(x, sanitized_test_case_name, 0, CCV_IO_BINARY_FILE, 0);
	assert(len < x->rows * x->cols * 4);
	ccv_dense_matrix_t* y = 0;
	ccv_read(data, &y, CCV_IO_ANY_STREAM, len);
	ccfree(data);
	ccv_dense_matrix_t* z = 0;
	ccv_read(sanitized_test_case_name, &z, CCV_IO_ANY_FILE, 0);
	FILE* fd = fopen(sanitized_test_case_name, "rb");
	fseek(fd, 0L, SEEK_END);
	size_t fsz = ftell(fd);
	fclose(fd);
	REQUIRE_EQ(fsz, len, "size of both memory and file should be the same");
	REQUIRE_MATRIX_EQ(y, z, "write nature.png to binary in file system and memory should be the same");
	ccv_matrix_free(z);
	ccv_matrix_free(y);
	ccv_matrix_free(x);
}

TEST_CASE("write binary to memory fail at small memory allocation")
{
	ccv_dense_matrix_t* x = 0;
	ccv_read("../../samples/nature.png", &x, CCV_IO_ANY_FILE);
	char* data = (char*)ccmalloc(100);
	size_t len = 100;
	int error = ccv_write(x, data, &len, CCV_IO_PLAIN_STREAM, 0);
	REQUIRE_EQ(error, CCV_IO_ERROR, "fail at write the file");
	ccfree(data);
	ccv_matrix_free(x);
}
#endif

#include "case_main.h"
