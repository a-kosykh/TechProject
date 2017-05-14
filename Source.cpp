#include <iostream>
#include "json.hpp"
#include <fstream>
#include <string>
#include <curl/curl.h>
#include <cstddef>
#include <vector>

using nlohmann::json;

int fileSize[2];


namespace ns {
	unsigned int commitAmount;
	struct commit {
		std::string sha;
		std::string login;
		std::string date;
		std::string message;
		std::vector<std::string> parents;

		commit(const json& j, unsigned int n) {
			this->sha = j[n]["sha"].get<std::string>();
			this->login = j[n]["author"]["login"].get<std::string>();
			this->date = j[n]["sha"].get<std::string>();
			this->message = j[n]["commit"]["message"].get<std::string>();
			this->parents.push_back(j[n]["parents"][0]["sha"].get<std::string>());
		}
	};

	std::vector<commit> commitList;

	void pushtoList(json& j, unsigned int n) {
		if (n > 25) {
			for (unsigned int i = 0; i < 25; ++i) {
				ns::commitList.push_back(ns::commit(j, i));
			}
		}
		else {
			for (unsigned int i = 0; i < n; ++i) {
				ns::commitList.push_back(ns::commit(j, i));
			}
		}
	}
	void getcommitAmount(json& j) {
		commitAmount = j[0]["contributions"].get<unsigned int>();
	}
}


std::string formattedJson(char *json)
{
	std::string pretty;

	if (json == NULL || strlen(json) == 0)
	{
		return pretty;
	}

	std::string str = std::string(json);
	bool        quoted = false;
	bool        escaped = false;
	std::string INDENT = "    ";
	int         indent = 0;
	int         length = (int)str.length();
	int         i;

	for (i = 0; i < length; i++)
	{
		char ch = str[i];

		switch (ch)
		{
		case '{':
		case '[':
			pretty += ch;

			if (!quoted)
			{
				pretty += "\n";

				if (!(str[i + 1] == '}' || str[i + 1] == ']'))
				{
					++indent;

					for (int j = 0; j < indent; j++)
					{
						pretty += INDENT;
					}
				}
			}

			break;

		case '}':
		case ']':
			if (!quoted)
			{
				if ((i > 0) && (!(str[i - 1] == '{' || str[i - 1] == '[')))
				{
					pretty += "\n";

					--indent;

					for (int j = 0; j < indent; j++)
					{
						pretty += INDENT;
					}
				}
				else if ((i > 0) && ((str[i - 1] == '[' && ch == ']') || (str[i - 1] == '{' && ch == '}')))
				{
					for (int j = 0; j < indent; j++)
					{
						pretty += INDENT;
					}
				}
			}

			pretty += ch;

			break;

		case '"':
			pretty += ch;
			escaped = false;

			if (i > 0 && str[i - 1] == '\\')
			{
				escaped = !escaped;
			}

			if (!escaped)
			{
				quoted = !quoted;
			}

			break;

		case ',':
			pretty += ch;

			if (!quoted)
			{
				pretty += "\n";

				for (int j = 0; j < indent; j++)
				{
					pretty += INDENT;
				}
			}

			break;

		case ':':
			pretty += ch;

			if (!quoted)
			{
				pretty += " ";
			}

			break;
		default:
			pretty += ch;

			break;
		}

	}
	std::size_t found = pretty.find_last_of(']');
	pretty.erase(found + 1, EOF);
	return pretty;
}

static void getJSON(char* url, char* filename, unsigned int count) {
	CURL  *curl; // указатель на объект cURL
	curl = curl_easy_init(); // запуск "легкого" хэндлера
	CURLcode res; // объект класса cURLcode

	if (curl) {
		struct curl_slist *chunk = NULL; // список на ноль
		chunk = curl_slist_append(chunk, "User-Agent: lalala"); // кастомный хэдер USERAGENT в список хэдеров
		res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk); // список отправляется в хэдер запроса
		curl_easy_setopt(curl, CURLOPT_URL, url); // ссылка, по которой обращаемся

		FILE *pF;
		pF = fopen(filename, "wb");
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, pF); // команда на копирование данных в файл

		res = curl_easy_perform(curl); // выполнение запросов 

		if (res != CURLE_OK) // проверка
			fprintf(stderr, "Could not download file: %s\n",
				curl_easy_strerror(res));
		else printf("SUCCESS: File has been dowloaded\n");

		rewind(pF);
		fseek(pF, 0, SEEK_END);
		fileSize[count] = ftell(pF); // получение размера файла
		curl_easy_cleanup(curl);
		fclose(pF);
	}
}

void toJSON(json& j, char* filename, unsigned int c) {
	std::ifstream fromFile(filename);
	char* line = new char[fileSize[c]];
	while (!fromFile.eof()) {
		for (unsigned int i = 0; i <= fileSize[c]; ++i) {
			fromFile >> line[i];
		}
	}
	fromFile.close();

	std::ofstream toFile(filename);
	toFile << formattedJson(line);
	toFile.close();

	std::ifstream finFile(filename);
	j << finFile;

	finFile.close();
}

int main() {
	char url1[] = { "https://api.github.com/repos/akosoj/Sem2Lab2/commits" };
	char url2[] = { "https://api.github.com/repos/akosoj/Sem2Lab2/contributors" };
	getJSON(url1, "repo.json", 0);
	getJSON(url2, "count.json", 1);
	
	json j1, j2;
	toJSON(j1, "repo.json", 0);
	toJSON(j2, "count.json", 1);

	

	///////////////////////// TESTING AREA /////////////////////////////////

	// Динамическое выделение памяти для каждого коммита
	/*

	unsigned int n;
	std::cin >> n;

	ns::commit *com = new ns::commit[n];

	for (unsigned int i = 0; i < n; ++i) {
	ns::from_json(j, com[i], i);
	std::cout << com[i].sha << std::endl;
	std::cout << com[i].login << std::endl;
	std::cout << com[i].date << std::endl;
	std::cout << com[i].message << std::endl;
	std::cout << "-----------------" << std::endl;
	}
	*/

	// Статическое выделение памяти под один коммит

	ns::getcommitAmount(j2);

	ns::pushtoList(j1, ns::commitAmount);
	

	for (auto i = ns::commitList.begin() ; i < ns::commitList.end(); ++i) {
		unsigned int n = 0;
		std::cout << "Commit number: " << ns::commitList.end() - i  << std::endl;
		std::cout << "Date: " << i->date << std::endl;
		std::cout << "Author: " << i->login << std::endl;
		std::cout << "Message: " << i->message << std::endl;
		std::cout << "SHA: " << i->sha << std::endl;
		
		std::cout << "--------------------------\n";
	}
	///////////////////// END OF TESTING AREA //////////////////////////////

	std::cout << std::endl;
	system("pause");
	return 0;
}