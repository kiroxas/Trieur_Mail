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

	for(auto& it : merged)
	{
		if(std::find(new_mails.begin(),new_mails.end(),it) != new_mails.end())
		{
			tmp += "<font color=\"red\"><b>" + it + "</b></font></br>";
		}
		else
			tmp += it + "</br>";
	}

	return tmp;
}

int main(int argc, char* argv[])
{
	std::ifstream conf("conf.txt");
	if(conf.fail())
	{
		std::cout << "N'as pas pu ouvrir : "<< argv[1] << std::endl;
		char c;
		std::cin >> c;
		return 1;
	}

	std::string filename1, filename2, output;

	std::getline(conf,filename1);
	std::getline(conf,filename2);
	std::getline(conf,output);

	std::ifstream file1(filename1);
	if(file1.fail())
	{
		std::cout << "N'as pas pu ouvrir : "<< filename1 << std::endl;
		char c;
		std::cin >> c;
		return 1;
	}

	std::ifstream file2(filename2);
	if(file2.fail())
	{
		std::cout << "N'as pas pu ouvrir : "<< filename2 << std::endl;
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

	std::ofstream of(output.empty() ? "merged_mails.html" : output);
	of << ToHtml(merged,new_mails);

	return 0;
}