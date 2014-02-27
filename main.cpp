#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>

std::string Usage()
{
	return "Usage : trieur.exe <ancien fichier> <nouveau fichier> <fusion des 2 fichiers>";
}

std::string ToHtml(const std::vector<std::string>& merged,const std::vector<std::string>& new_mails)
{
	std::string tmp;

	for(std::vector<std::string>::const_iterator it = merged.begin(), end = merged.end(); it != end; ++it)
	{
		if(std::find(new_mails.begin(),new_mails.end(),*it) != new_mails.end())
		{
			tmp += "<font color=\"red\"><b>" + *it + "</b></font></br>";
		}
		else
			tmp += *it + "</br>";
	}

	return tmp;
}

int main(int argc, char* argv[])
{
	if(argc == 1)
	{
		std::cout << Usage() << std::endl;
		char c;
		std::cin >> c;
		return 1;
	}
	
	std::ifstream file1(argv[1]);
	if(file1.bad())
	{
		std::cout << "N'as pas pu ouvrir : "<< argv[1] << std::endl;
		char c;
		std::cin >> c;
		return 1;
	}
	std::ifstream file2(argv[2]);
	if(file2.bad())
	{
		std::cout << "N'as pas pu ouvrir : "<< argv[2] << std::endl;
		char c;
		std::cin >> c;
		return 1;
	}

	std::vector<std::string> old_mails;
	std::vector<std::string> new_mails;
	std::string current_mail;
	
	while(std::getline(file1,current_mail))
	{
		std::transform(current_mail.begin(),current_mail.end(),current_mail.begin(),::tolower);
		if(!current_mail.empty() && std::find(old_mails.begin(),old_mails.end(),current_mail) == old_mails.end())
			old_mails.push_back(current_mail);
	}

	while(std::getline(file2,current_mail))
	{
		std::transform(current_mail.begin(),current_mail.end(),current_mail.begin(),::tolower);
		if(!current_mail.empty() && std::find(new_mails.begin(),new_mails.end(),current_mail) == new_mails.end()
			                     && std::find(old_mails.begin(),old_mails.end(),current_mail) == old_mails.end())
			new_mails.push_back(current_mail);
	}

	std::sort(old_mails.begin(),old_mails.end());
	std::sort(new_mails.begin(),new_mails.end());

	std::vector<std::string> merged(old_mails.size() + new_mails.size());
	std::vector<std::string>::const_iterator it = std::set_union(old_mails.begin(), old_mails.end(), new_mails.begin(), new_mails.end(), merged.begin());

	merged.resize(it - merged.begin());

	std::ofstream of("merged_mails.html");
	of << ToHtml(merged,new_mails);

	return 0;
}