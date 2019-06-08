#include <iostream>
#include <limits>
#include "big_integer.h"


typedef big_integer bi;

void testShort(std::string s) {
    bi a(s);
    bi c;
    c = a;
    std::cout << to_string(a) << " " << to_string(c) << std::endl;
}

void testLong(std::string s) {
    bi a(s);
    bi c("340282366920938463463374607431768211456000");
    c *= c;
    c *= c;
    std::cout << "assignment" << std::endl;
    c = a;
    std::cout << to_string(a) << " " << to_string(c) << std::endl;
}

void test() {


    big_integer a("571532341821635832718390731269770972891275082227359594556626939613946606239894020456747298240");
    bi b("92011082625151565178793656");
////    big_integer c("-100000000000000000000000000000000000000000000000000000");
////    bi b = a;
    bi c;
    c = a / b;
    c.print("c");
    std::cout << to_string(c) << std::endl;

    bi d = a * a * a;
    a = d / a / a;
    d = b * b * b;
    b = d / b / b;
    c = a / b;
    c.print("c");
    std::cout << to_string(c) << std::endl;
}

void test3() {
    bi a("1000000000000000"),b("100000000000000000000000000000");
//    b = big_integer("-32450653018688485974567205773955356");
//    a = big_integer("384450496101304526117020440");
    std::swap(a, b);
//    b = big_integer("-32450653018688485974567205773955356");
//    a = big_integer(b);
    auto sa = to_string(a);
    auto sb = to_string(b);
//    auto sb = to_string(b);
//    auto sab = to_string(a + b);
//    auto sba = to_string(b + a);
    std::cout << sa << " " << sb << std::endl;
}

void test2() {
    bi b("1000000000000000000");
    bi a("-1000000000000000000000000000000000000");
//    bi a("3732412931757522912270045895346908707635528047235004628992");
//    bi b("32451201800");

    std::cout << to_string(a) << " * " << to_string(b) << std::endl;
    bi ab = a * b;
    if (ab / a != b) {
        std::string sa = to_string(a);
        std::string sb = to_string(b);
        std::string sab = to_string(ab);
        std::string actual = to_string(ab / a);
        std::cout << "failed " << sab << " / " << sa << " != " << sb << "\n" <<
                  "actual " << actual;
        std::cout << "failed" << std::endl;

    } else {
        std::string sa = to_string(a);
        std::string sb = to_string(b);
        std::string sab = to_string(ab);
        std::string actual = to_string(ab / a);
        std::cout << sab << " / " << sa << " == " << sb << std::endl;
    }


}

int main() {
//    testShort("1000");
//    testShort("184467440737095516160");
//    testShort("340282366920938463463374607431768211456000");
//    testLong("212709971126891681359335675390407475200");
    test3();
//    testLong("1000");
//    testLong("184467440737095516160");
//    testLong("340282366920938463463374607431768211456000");

    //    big_integer d(a);
//    big_integer e(b);
//    big_integer f(c);
//    std::cout << (f == d / e) << std::endl;
//
//    big_integer d("36893488147419103232000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
//    big_integer e("23158417847463239084714197001737581570653996933128112807891516801582625927987200000000000000000000000000000000000000");
//
//    std::cout << ((d >= e) ? ">=" : "<") << std::endl;
////    c = b + a;
////    c.print("c");
////    std::cout << to_string(c) << std::endl;


}