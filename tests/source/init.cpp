#include <testclass.hpp>
#include <catch.hpp>

SCENARIO ("check","[!mayfail]")
{
	char url1[] = { "https://api.github.com/repos/DavidTsyg/Sem2_Lab7/commits?client_id=fcca22e4d885095a01e1&client_secret=5865c57c862238c4e83ea6f97e86ce24e85368fd" };
	char filename1[] = {"repo.json"};
	char url2[] = { "https://api.github.com/repos/DavidTsyg/Sem2_Lab7/contributors?client_id=fcca22e4d885095a01e1&client_secret=5865c57c862238c4e83ea6f97e86ce24e85368fd" };
	char filename2[] = {"count.json"};
	
	json j1, j2;
	
	getJSON(url1, filename1);
	toJSON(j1, filename1, 2);
	getJSON(url2, filename2);
	toJSON(j2, filename2, 2);
	ns::getcommitAmount(j2);
	ns::pushtoList(j1, ns::commitAmount);
	
	REQUIRE (ns::commitList.size() == 25);
}