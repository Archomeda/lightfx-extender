#include "CppUnitTest.h"

// Project includes
#include "../../src/Utils/String.h"


using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace lightfx::utils;

namespace lightfx_tests {
    namespace utils {

        TEST_CLASS(StringTest) {
public:

    TEST_METHOD(WstringToString) {
        wstring input = L"test";

        string expected = "test";
        string actual = wstring_to_string(input);
        Assert::IsTrue(expected == actual);
    }

    TEST_METHOD(WstringEmptyToString) {
        wstring input = L"";

        string expected = "";
        string actual = wstring_to_string(input);
        Assert::IsTrue(expected == actual);
    }

    TEST_METHOD(StringToWstring) {
        string input = "test";

        wstring expected = L"test";
        wstring actual = string_to_wstring(input);
        Assert::IsTrue(expected == actual);
    }

    TEST_METHOD(StringEmptyToWstring) {
        string input = "";

        wstring expected = L"";
        wstring actual = string_to_wstring(input);
        Assert::IsTrue(expected == actual);
    }

    TEST_METHOD(SplitString) {
        string str = "1,2";
        char delim = ',';

        vector<string> expected = { "1", "2" };
        vector<string> actual = split(str, delim);
        Assert::IsTrue(expected == actual);
    }

    TEST_METHOD(SplitStringAmount) {
        string str = "1,2,3,4";
        char delim = ',';

        vector<string> expected = { "1", "2", "3,4" };
        vector<string> actual = split(str, delim, 3);
        Assert::IsTrue(expected == actual);
    }

    TEST_METHOD(SplitStringMissingDelim) {
        string str = "1,2";
        char delim = '.';

        vector<string> expected = { "1,2" };
        vector<string> actual = split(str, delim);
        Assert::IsTrue(expected == actual);
    }

    TEST_METHOD(SplitStringTrailingEmpty) {
        string str = "1,2,";
        char delim = ',';

        vector<string> expected = { "1", "2" };
        vector<string> actual = split(str, delim);
        Assert::IsTrue(expected == actual);
    }

    TEST_METHOD(SplitStringEmpty) {
        string str = "";
        char delim = ' ';

        vector<string> expected = {};
        vector<string> actual = split(str, delim);
        Assert::IsTrue(expected == actual);
    }

    TEST_METHOD(SplitWstring) {
        wstring str = L"1,2";
        wchar_t delim = ',';

        vector<wstring> expected = { L"1", L"2" };
        vector<wstring> actual = split(str, delim);
        Assert::IsTrue(expected == actual);
    }

    TEST_METHOD(SplitWstringAmount) {
        wstring str = L"1,2,3,4";
        wchar_t delim = ',';

        vector<wstring> expected = { L"1", L"2", L"3,4" };
        vector<wstring> actual = split(str, delim, 3);
        Assert::IsTrue(expected == actual);
    }

    TEST_METHOD(SplitWstringMissingDelim) {
        wstring str = L"1,2";
        wchar_t delim = '.';

        vector<wstring> expected = { L"1,2" };
        vector<wstring> actual = split(str, delim);
        Assert::IsTrue(expected == actual);
    }

    TEST_METHOD(SplitWstringTrailingEmpty) {
        wstring str = L"1,2,";
        wchar_t delim = ',';

        vector<wstring> expected = { L"1", L"2" };
        vector<wstring> actual = split(str, delim);
        Assert::IsTrue(expected == actual);
    }

    TEST_METHOD(SplitWstringEmpty) {
        wstring str = L"";
        wchar_t delim = ' ';

        vector<wstring> expected = {};
        vector<wstring> actual = split(str, delim);
        Assert::IsTrue(expected == actual);
    }

        };
    }
}
