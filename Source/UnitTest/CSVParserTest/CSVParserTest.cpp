#include "UnitTestPCH.h"
#include <boost/test/unit_test.hpp>
#include <boost/tokenizer.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/classic.hpp>
#include <boost/spirit/include/karma.hpp>

#include "FastCSV.h"

#include <iostream>     // cout, endl
#include <fstream>      // fstream
#include <vector>
#include <string>
#include <algorithm>    // copy
#include <iterator>     // ostream_operator

BOOST_AUTO_TEST_SUITE(CSVParsertTestSuit)

namespace qi = boost::spirit::qi;

//const std::string s = R"(a,"b,b","c,""c","d""""","e""e""","f
//f",g\g,h/\,i\\i,j//j,\n,\r,\r\n,\t)";

const std::string s = R"(a,"b,b","c,""c","d""""","e""e""","f
f")";

BOOST_AUTO_TEST_CASE(BoostTokenizerSpiritTestCase)
{
    // Tokenizer
    typedef boost::tokenizer< boost::escaped_list_separator<char>, std::string::const_iterator, std::string> Tokenizer;
    boost::escaped_list_separator<char> seps('\\', ',', '\"');
    Tokenizer tok(s, seps);
    for (auto i : tok)
        std::cout << i << "\n";
    std::cout << "\n";

    // Boost Spirit Qi
    qi::rule<std::string::const_iterator, std::string()> quoted_string = '"' >> *(qi::char_ - '"') >> '"';
    qi::rule<std::string::const_iterator, std::string()> valid_characters = qi::char_ - '"' - ',';
    qi::rule<std::string::const_iterator, std::string()> item = *(quoted_string | valid_characters);
    qi::rule<std::string::const_iterator, std::vector<std::string>()> csv_parser = item % ',';

    std::string::const_iterator s_begin = s.begin();
    std::string::const_iterator s_end = s.end();
    std::vector<std::string> result;

    bool r = boost::spirit::qi::parse(s_begin, s_end, csv_parser, result);
    assert(r == true);
    assert(s_begin == s_end);

    for (auto i : result)
        std::cout << i << std::endl;
    std::cout << "\n";
}
BOOST_AUTO_TEST_CASE(BoostTokenizerTestCase)
{
    typedef boost::tokenizer< boost::escaped_list_separator<char> > Tokenizer;

    std::vector< std::string > vec;
    Tokenizer tok(s);
    vec.assign(tok.begin(), tok.end());

    std::copy(vec.begin(), vec.end(),
        std::ostream_iterator<std::string>(std::cout, "|"));
}
BOOST_AUTO_TEST_CASE(FastCSVTestCase)
{
    #define COLUMN_CNT 6
    io::CSVReader< COLUMN_CNT, io::trim_chars<' ', '\t'>, io::double_quote_escape<',', '\"'> > in("test.csv");
    in.read_header(io::ignore_extra_column, "c0", "c1", "c2", "c3", "c4", "c5");
    std::string c[COLUMN_CNT];

    while (in.read_row(c[0], c[1], c[2], c[3], c[4], c[5]))
    {
        for (int i = 0; i < COLUMN_CNT; ++i)
        {
            std::cout << c[i] << ",";
        }
        std::cout << std::endl;
    }
}
BOOST_AUTO_TEST_SUITE_END()