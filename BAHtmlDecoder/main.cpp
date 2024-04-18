//
//  main.cpp
//  BAHtmlDecoder
//
//  Created by arvinnie on 2024/4/18.
//

#include <iostream>
#include <cassert>
#include "BAHtmlDecoder.h"

int main(int argc, const char * argv[]) {
    assert("abc<>>xyz" == BAHtmlDecoder::instance().decode("abc&#60;&#x3e;&#X3E;xyz"));
    assert("abc&#1a3;&#xcgdef;&#;&#z;xyz" == BAHtmlDecoder::instance().decode("abc&#1a3;&#xcgdef;&#;&#z;xyz"));
    assert("<div>&ltabc&lt<habc@test.com></div>" == BAHtmlDecoder::instance().decode("<div>&ltabc&lt&#60;habc@test.com&gt;</div>"));
    return 0;
}
