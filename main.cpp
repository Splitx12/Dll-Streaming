#include "Stream.h"
#include "crypt.h"
int main(int argc, wchar_t* argv[])
{

	fs::path CurrentDirectory = fs::path(argv[0]).remove_filename();

	fs::path dropped = Encrypt("");

	//if theres no arguments given (no file being dropped on the exe)
	if (!argv[1])
	{
		{
			std::cout << Encrypt("\nenter \"start\" to test streaming (drops to disk)\n");

			std::string input;

			std::cin >> input;

			if (input == Encrypt("start"))
			{
				TestDW(CurrentDirectory);

				std::cout << Encrypt("streamed and saved at ") << CurrentDirectory / Encrypt("file.dll") << "\n\n";
				system(Encrypt("pause"));
				exit(0);
			}
			else if (input == Encrypt("example-usage"))
			{
				//call GetFile to stream and decrypt the file

				std::string buffer = GetFile();

				//now use this buffer for your injector, mapper...

				//in this case we will drop it to disk

				std::ofstream ofs;
				ofs.open(CurrentDirectory / Encrypt("file.dll"), std::ios::binary);
				std::copy(buffer.begin(), buffer.end(), std::ostream_iterator<char>(ofs));
				ofs.close();

				buffer.clear();

				std::cout << Encrypt("example done: streamed and saved at ") << CurrentDirectory / Encrypt("file.dll") << "\n\n";
				system(Encrypt("pause"));
				exit(0);
			}

			else
			{
				std::cout << Encrypt("retard");
				system(Encrypt("pause"));
				exit(0);
			}
		}
	}

	else dropped = fs::path(argv[1]);

	//if file is the first encrypted file, decrypt it and build it to the original file
	if (dropped.filename() == Encrypt("1"))
	{
		DLF(dropped);
		std::cout << Encrypt("File decrypted\nEnter to exit\n");
		system(Encrypt("pause"));
		exit(0);
	}

	//checks if file is a dll, if so starts encryption
	else if (dropped.has_extension() || dropped.extension() == Encrypt(".dll"))
	{
		EncryptFile(dropped);
		std::cout << Encrypt("Files encrypted\nEnter to exit\n");
		system(Encrypt("pause"));
		exit(0);
	}

	//if its not a dll file, print a error
	else if (dropped.has_extension() || dropped.extension() != Encrypt(".dll"))
	{
		std::cout << Encrypt("Invalid file, must be .dll\nEnter to exit\n");
		system(Encrypt("pause"));
		exit(0);
	}

	else
	{
		std::cout << Encrypt("retard\n");

		system(Encrypt("pause"));
		exit(0);
	}
}