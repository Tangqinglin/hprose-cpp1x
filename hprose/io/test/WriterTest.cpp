/**********************************************************\
|                                                          |
|                          hprose                          |
|                                                          |
| Official WebSite: http://www.hprose.com/                 |
|                   http://www.hprose.org/                 |
|                                                          |
\**********************************************************/

/**********************************************************\
 *                                                        *
 * WriterTest.cpp                                         *
 *                                                        *
 * hprose writer test for cpp.                            *
 *                                                        *
 * LastModified: Oct 17, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#include <hprose/io/Writer.h>

#include <gtest/gtest.h>

#include <random>

#define T(value, expected) { \
    std::ostringstream stream; \
    hprose::io::Writer writer(stream); \
    writer.serialize(value); \
    EXPECT_EQ(stream.str(), expected); \
}

TEST(Writer, SerializeNull) {
    T(nullptr, "n");
}

TEST(Writer, SerializeBool) {
    T(true, "t");
    T(false, "f");
}

TEST(Writer, SerializeDigit) {
    for (int i = 0; i <= 9; i++) {
        T(i, std::to_string(i));
    }
}

TEST(Writer, SerializeInteger) {
    std::random_device rd;

    std::uniform_int_distribution<int32_t> dis1(10); 
    for (int i = 0; i < 100; i++) {
        int32_t x = dis1(rd);
        T(x, "i" + std::to_string(x) + ";");
    }

    std::uniform_int_distribution<int64_t> dis2(static_cast<int64_t>(std::numeric_limits<int32_t>::max()) + 1);
    for (int i = 0; i < 100; i++) {
        int64_t x = dis2(rd);
        T(x, "l" + std::to_string(x) + ";");
    }
}

TEST(Writer, SerializeInt8) {
    for (signed char i = 0; i <= 9; i++) {
        T(i, std::to_string(i));
    }

    for (signed char i = 9; i < 127; i++) {
        signed char x = i + 1;
        T(x, "i" + std::to_string(x) + ";");
    }

    for (signed char i = -128; i < 0; i++) {
        T(i, "i" + std::to_string(i) + ";");
    }
}

TEST(Writer, SerializeFloat) {
    T(std::numeric_limits<float>::quiet_NaN(), "N");
    T(std::numeric_limits<float>::infinity(), "I+");
    T(-std::numeric_limits<float>::infinity(), "I-");
    T(3.14159265358979323846f, "d3.14159;");

    T(std::numeric_limits<double>::quiet_NaN(), "N");
    T(std::numeric_limits<double>::infinity(), "I+");
    T(-std::numeric_limits<double>::infinity(), "I-");
    T(3.14159265358979323846, "d3.14159265358979;");

    T(std::numeric_limits<long double>::quiet_NaN(), "N");
    T(std::numeric_limits<long double>::infinity(), "I+");
    T(-std::numeric_limits<long double>::infinity(), "I-");
    T(3.14159265358979323846l, "d3.14159265358979324;");
}

TEST(Writer, SerializeComplex) {
    T(std::complex<float>(100.f), "d100;");
    T(std::complex<float>(0, 100.f), "a2{d0;d100;}");

    T(std::complex<double>(100.f), "d100;");
    T(std::complex<double>(0, 100.f), "a2{d0;d100;}");

    T(std::complex<long double>(100.f), "d100;");
    T(std::complex<long double>(0, 100.f), "a2{d0;d100;}");
}

TEST(Writer, SerializeString) {
    T(std::string(u8""), "e");
    T(std::string(u8"π"), "uπ");
    T(std::string(u8"你"), "u你");
    T(std::string(u8"你好"), R"(s2"你好")");
    T(std::string(u8"你好啊,hello!"), R"(s10"你好啊,hello!")");
    T(std::string(u8"🇨🇳"), "s4\"🇨🇳\"");

    T(std::wstring(L""), "e");
    T(std::wstring(L"π"), "uπ");
    T(std::wstring(L"你"), "u你");
    T(std::wstring(L"你好"), R"(s2"你好")");
    T(std::wstring(L"你好啊,hello!"), R"(s10"你好啊,hello!")");
    T(std::wstring(L"🇨🇳"), "s4\"🇨🇳\"");

    T(std::u16string(u""), "e");
    T(std::u16string(u"π"), "uπ");
    T(std::u16string(u"你"), "u你");
    T(std::u16string(u"你好"), R"(s2"你好")");
    T(std::u16string(u"你好啊,hello!"), R"(s10"你好啊,hello!")");
    T(std::u16string(u"🇨🇳"), "s4\"🇨🇳\"");

    T(std::u32string(U""), "e");
    T(std::u32string(U"π"), "uπ");
    T(std::u32string(U"你"), "u你");
    T(std::u32string(U"你好"), R"(s2"你好")");
    T(std::u32string(U"你好啊,hello!"), R"(s10"你好啊,hello!")");
    T(std::u32string(U"🇨🇳"), "s4\"🇨🇳\"");
}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
