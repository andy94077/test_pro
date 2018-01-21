#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <limits>
using namespace std;
#ifdef _WIN32
#define SYSTEMPAUSE system("pause")
	const string diff_path = "bin\\diff ", os_delim="\\", shell_script="out.bat";
#else
#define SYSTEMPAUSE system("bash -c \"read -n1 -rsp $\'Press any key to continue...\n\'\"")
	const string diff_path = "diff ",os_delim="/",shell_script="out.sh";
#endif // __WIN32

//#define DEBUG

inline int system(const string& str)
{
	return system(str.c_str());
}
inline void rm_space_quotation(string& str)
{
	str.erase(0,str.find_first_not_of(" \t\""));
	str.erase(str.find_last_not_of(" \t\"") + 1);
}

void read_default(string& exe_path, string& file_dir, string& filename_extension, int& start_num, int& end_num, string& diff_option)
{
	string dat_path = "default.dat";
	ifstream dat(dat_path);
	while(dat.fail())
	{
		cerr << "can not find\"default.dat\"" << endl;
		cout << "please enter the full path of \"default.dat\":" << endl;
		getline(cin, dat_path);
		
		//remove the space and quote
		rm_space_quotation(dat_path);

		dat.open(dat_path);
	}

	const string error_message = "format error";
	string program_name;
	float version;
	dat >> program_name >> version;
	if (program_name != "test_pro" || !dat.good())
	{
		cerr << "default.dat"<<error_message<<'\n';
		return;
	}
	try
	{
		dat.ignore(numeric_limits<streamsize>::max(),'\n');//next line
		if(!getline(dat, exe_path))							throw "exe path";
		if(!getline(dat,file_dir))								throw "file directory";
		if(!getline(dat, filename_extension))		throw "filename extension";
		if(!(dat >> start_num >> end_num))		throw "range";
	}
	catch (const char *err)
	{
		cerr << err <<error_message<< '\n';
	}
	
	if (version >= 2.0)
	{
		dat.ignore(numeric_limits<streamsize>::max(),'\n');//next line
		getline(dat, diff_option);
	}
	dat.close();
	//remove the space and quote
	rm_space_quotation(exe_path);
	//remove the space and quote
	rm_space_quotation(file_dir);

}
void read_options(string& exe_path, string& file_dir, string& filename_extension, int& start_num, int& end_num, string &single_file, string& diff_option)
{
	string option_key,str;
	while (cin>>option_key)
	{
		if (option_key == "run" || option_key == "r")
			break;
		if (option_key[0] != '-')
		{
			cerr << "unknown option\n";
			continue;
		}

		getline(cin, str);
		//remove the space and quote
		rm_space_quotation(str);

		if (option_key == "-e")
			exe_path = str;
		else if (option_key == "-d")
			file_dir = str;
		else if (option_key == "-f")
			filename_extension = str;
		else if (option_key == "-r")
		{
			stringstream start_end_ss(str);
			start_end_ss >> start_num >> end_num;
			//if start_num > end_num, then the for checking part won't be executed
			if (start_num > end_num)
			{
				start_num = 1;
				end_num = 0;
			}
		}
		else if (option_key == "-s")
			single_file+=' '+ str;
		else if (option_key == "-c")
				diff_option = str;
		else
			cerr << "unknown option\n";
	}
#ifndef _WIN32
#include <cstdlib>
	if (exe_path[0]=='~')
	{	
		exe_path.erase(exe_path.begin());
		exe_path.insert(0, getenv("HOME"));
	}
	if(file_dir[0]=='~')
	{
		file_dir.erase(file_dir.begin());
		file_dir.insert(0, getenv("HOME"));
	}
#endif //_WIN32
}

bool write_bat(const string& exe_path, const string& file_dir, const string& filename_extension, const int start_num, const int end_num,const string& single_files )
{	
#ifdef _WIN32
	const string cd = "cd /d \""+file_dir+'\"';
#else
	const string cd=(file_dir.find(' ')==string::npos)?("cd "+file_dir):("cd \""+file_dir+'\"');
#endif // _WIN32
#ifdef DEBUG
	cout<<"cd#"<<cd<<"#\n";
	SYSTEMPAUSE;
#endif //DEBUG
	if(system(cd))//failed to cd
		return false;

	string script_path=file_dir + os_delim+ shell_script;
	ofstream script(script_path, ios::out | ios::trunc);
	if (script.fail())
	{
		cerr << "can not open " << script_path <<'\n' ;
		return false;
	}

#ifndef _WIN32
	script<<"set -x\n";//echo shell commands
#endif //_WIN32
	script << cd <<'\n';
#ifdef _WIN32
	const string to_do = ") do \"" + exe_path + "\" <%%A" + filename_extension + " >%%A_out.txt\n";
	script << "for /L %%A in (" << start_num << ",1," << end_num<<to_do;
	script <<"for %%A in ("<< single_files<< to_do;
#else
	const string to_do = "; do \"" + exe_path + "\" <\"$a\"" + filename_extension + " >\"$a\"_out.txt; done\n";
	script << "for a in $(seq " << start_num << ' ' << end_num <<')'<< to_do;
	script << "for a in " << single_files << to_do;
#endif // _WIN32
	script.close();
	return true;
}

void compare_answer(ifstream& your_ans, ifstream& correct_ans, const string& filepath, const string& correct_ans_filepath)
{
	bool accepted = true;
	while (!your_ans.eof() || !correct_ans.eof())
	{
		string ans, right_ans;
		getline(your_ans,ans);
		getline(correct_ans, right_ans);

		if (ans != right_ans)
		{
			accepted = false;
			string str;
			your_ans.seekg(0);
			correct_ans.seekg(0);
			
			cout <<"\nwrong answer:" << filepath.substr(filepath.find_last_of(os_delim)+1) << '\n';
			cout << "your answers:\n";
			while (getline(your_ans, str))
				cout << str << '\n';

			cout << "correct answers:\n";
			while (getline(correct_ans, str))
				cout << str << '\n';
			cout << '\n';
			break;
		}
	}
	if (accepted)
		cout << filepath.substr(filepath.find_last_of(os_delim) + 1) << "\taccepted\n";
}
void diff_answer(ifstream& your_ans, ifstream& correct_ans,const string &diff_option, const string& my_ans_path, const string& correct_ans_path)
{
	//enclose the path in quotes
	string test = diff_path  + '\"' + correct_ans_path + "\" \"" + my_ans_path + "\" "+ diff_option;
//if it's windows, enclose the whole command in quotes
#ifdef _WIN32
	test.insert(test.begin(),'\"');
	test+='\"';
#endif //_WIN32
#ifdef DEBUG
	cout << "test#" << test << "#\n";
#endif // DEBUG

	if (system(test))
		cout << '\n' << my_ans_path.substr(my_ans_path.find_last_of(os_delim)+1)<< "\twrong answer\n\n";
	else
		cout << my_ans_path.substr(my_ans_path.find_last_of(os_delim)+1) << "\taccepted\n";
}
void init_check_ans(const string &file_dir, const string &filename, const string &diff_option)
{
	ifstream my_ans, correct_ans;

	string my_ans_path = file_dir + os_delim + filename + "_out.txt";
	my_ans.open(my_ans_path);

	string correct_ans_path = file_dir + os_delim + filename + ".out";
	correct_ans.open(correct_ans_path);

	if (my_ans.fail())
		cerr << "can not open " << my_ans_path << '\n';
	else if (correct_ans.fail())
		cerr << "can not open " << correct_ans_path << '\n';
	else if (diff_option == "n" || diff_option == "N")
		compare_answer(my_ans, correct_ans, my_ans_path, correct_ans_path);
	else
		diff_answer(my_ans, correct_ans, diff_option, my_ans_path, correct_ans_path);

	my_ans.close();
	correct_ans.close();
}

int main()
{
	string exe_path, file_dir, filename_extension, diff_option="n";
	string str = "";
	int start_num, end_num;

	read_default(exe_path, file_dir, filename_extension, start_num, end_num, diff_option);

	cout << "options:\n";
	cout << "-e\tThe full path to the exe\n";
	cout << "-d\tSet directorty of test files \n";
	cout << "-f\tSet the file extension of test files\n";
	cout << "-r\tSet the start file name and the end file name.\n\tThe program will test them in sequence. If start > end, the program won't test them.\n";
	cout << "-s\tInput arbitrary test files. The program will test them one by one.\n";
	cout << "-c\tSet the option of diff. If it is \"N\" or \"n\", the program will use\n\tits own compare function, which is less powerful.\n";
	cout << "enter \"run\" or \"r\" to start test_pro\n";

	string single_file;
	read_options(exe_path, file_dir, filename_extension, start_num, end_num,single_file, diff_option);

#ifdef DEBUG
	cout << "exe_path#" << exe_path << "#\n";
	cout << "file_dir#"<<file_dir<< "#\n";
	cout << "filename_extension#" << filename_extension<< "#\n";
	cout << "start_num#" << start_num<< "#end_num#"<<end_num<<"#\n";
	cout << "single_file#" << single_file<< "#\n";
	cout << "diff_option#" << diff_option << "#\n";
	SYSTEMPAUSE;
#endif
	
	//build script
	if (!write_bat(exe_path, file_dir, filename_extension, start_num, end_num, single_file))
	{
		cerr << "the program will be terminated\n";
		SYSTEMPAUSE;
		exit(8);
	}

	//use script
#ifdef _WIN32
	system('\"'+file_dir+ os_delim+ shell_script+'\"');
#else
	system("bash \"" + file_dir + os_delim + shell_script + '\"');
#endif // _WIN32
	cout << '\n';

#ifdef DEBUG
	cout << "ready to check answers\n";
	SYSTEMPAUSE;
	cout << endl;
#endif

	//check continuous files
	for (int i = start_num; i <= end_num; i++)
		init_check_ans(file_dir, to_string(i), diff_option);

	//check scattered files
	stringstream single_file_ss(single_file);
	while (single_file_ss >> str)
		init_check_ans(file_dir, str, diff_option);

	SYSTEMPAUSE;
	return 0;
}
