#include <gtest/gtest.h>
#include <sstream>
#include <fstream>
#include <map>
#include <set>
#include <vector>

#define TESTING_MODE
#include "main.cpp"

TEST(Test, PalikaEiluteBeMarkdown) {
    EXPECT_EQ(stripMarkdown("Paprastas tekstas"), "Paprastas tekstas");
}

TEST(Test, PasalinaURLSkliaustuose) {
    std::string result = stripMarkdown("[Žemė](https://lt.wikipedia.org/wiki/Zeme)");
    EXPECT_NE(result.find("Žemė"), std::string::npos);
    EXPECT_EQ(result.find("https://"), std::string::npos);
}

TEST(Test, PakeiciaLauztinius_I_Tarpus) {
    std::string result = stripMarkdown("[žodis]");
    EXPECT_NE(result.find("žodis"), std::string::npos);
    EXPECT_EQ(result.find('['), std::string::npos);
    EXPECT_EQ(result.find(']'), std::string::npos);
}

TEST(Test, PaliekaRaides) {
    EXPECT_EQ(punctuation("žodis"), "žodis");
    EXPECT_EQ(punctuation("abc"), "abc");
}

TEST(Test, PasalinaASCIIBruksni) {
    EXPECT_EQ(punctuation("-"), "");
    EXPECT_EQ(punctuation("žo-dis"), "žodis");
}

TEST(Test, PasalinaEnDash) {
    std::string enDash = "\xE2\x80\x93";
    EXPECT_EQ(punctuation(enDash), "");
    EXPECT_EQ(punctuation("a" + enDash + "b"), "ab");
}

TEST(Test, PasalinaEmDash) {
    std::string emDash = "\xE2\x80\x94";
    EXPECT_EQ(punctuation(emDash), "");
}

TEST(Test, PasalinaMinusoZenkla) {
    std::string minus = "\xE2\x88\x92";
    EXPECT_EQ(punctuation(minus), "");
    EXPECT_EQ(punctuation(minus + "km"), "km");
}

TEST(Test, PalikaLietuviskasRaides) {
    EXPECT_EQ(punctuation("šiaurės"), "šiaurės");
    EXPECT_EQ(punctuation("Žemė"), "Žemė");
    EXPECT_EQ(punctuation("ąčęėįšųūž"), "ąčęėįšųūž");
}

TEST(Test, KonvertuojaAscii) {
    EXPECT_EQ(toLower("ABC"), "abc");
    EXPECT_EQ(toLower("Žemė"), "Žemė");
}

TEST(Test, MisrusAtvejis) {
    EXPECT_EQ(toLower("Hello"), "hello");
    EXPECT_EQ(toLower("HELLO WORLD"), "hello world");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}