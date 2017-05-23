#include <testclass.hpp>
#include <catch.hpp>

SCENARIO("ConstrDefa") {
	Complex comp;
	REQUIRE(comp.getReal() == 0);
	REQUIRE(comp.getIm() == 0);
}

SCENARIO("constructor with params") {
	Complex comp(1, 2);
	REQUIRE(comp.getReal() == 1);
	REQUIRE(comp.getIm() == 2);
}

SCENARIO("copy constructor") {
	Complex comp(1, 2);
	Complex copy(comp);
	REQUIRE(copy.getReal() == 1);
	REQUIRE(copy.getIm() == 2);
}

SCENARIO("operator*") {
	Complex c1(7, 6);
	Complex c2(5, 4);
	Complex c3(11, 58);
	REQUIRE((c1*c2)==c3);
}

SCENARIO("operator/") {
	Complex c1(7, 6);
	Complex c2(7, 6);
	Complex c3(1, 0);
	REQUIRE((c1/c2)==c3);
}

SCENARIO("operator +=") {
	Complex c1(13, 1); 
	Complex c2(7, 6);
	Complex c3(20, 7);
	REQUIRE((c1+=c2)==c3);
}

SCENARIO("operator -=") {
	Complex c1(13, 10); 
	Complex c2(7, 6);
	Complex c3(6, 4);
	REQUIRE((c1-=c2)==c3);
}

SCENARIO("operator *=") {
	Complex c1(7, 6); 
	Complex c2(5, 4);
	Complex c3(11, 58);
	REQUIRE((c1*=c2)==c3);
}

SCENARIO("operator /=") {
	Complex c1(7, 6); 
	Complex c2(7, 6);
	Complex c3(1, 0);
	REQUIRE((c1/=c2)==c3);
}

SCENARIO("operator =") {
	Complex c1(3, 3); 
	Complex c2=c1;
	REQUIRE(c2==c1);
}

SCENARIO("operator ==") {
	Complex c1(3, 1); 
	Complex c2(3, 1);
	REQUIRE(c1==c2);
}
SCENARIO("operator div0","[!mayfail]") {
	Complex c1(7, 6);
	Complex c2(7, 0);
	Complex c3(1, 0);
	REQUIRE((c1/c2)==c3);
}