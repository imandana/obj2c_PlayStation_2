/*
 *  Copyright 2020 Imandana Rahimaswara
 *
 * Released under the MIT license. See the accompanying LICENSE file
 * or visit <https://opensource.org/licenses/MIT>
 */

/*
 * \file obj2c.cpp
 * \author Imandana Rahimaswara
 *
 * Version 0.1
 *
 * Convert OBJ data to C array for Playstation 2
 * Mengkonversi data OBJ ke Array C dalam bentuk header(.h)
 */

#include <iostream>
#include <fstream>
#include <sstream>


void PointsFilter(std::string *, std::string *, std::string*);
void VerticesFilter(std::string *, std::string *, std::string*);
void UVSFIlter(std::string *, std::string *, std::string*);

int section;

int main(int argc, char* argv[])
{
	if(argc == 1){
		std::cout << "Error: No Input.\nUsage : obj2c <object>.obj" << std::endl;
		getchar();
		return -1;
	}

	std::string filename = argv[1];

	std::ifstream objData;
	objData.open(filename.c_str(), std::ifstream::in);

	// Name Extract (name.obj) to (name)
	filename = filename.substr( 0, (filename.size()-4));

	std::string strInput;
	std::string strOutput;

	int countVertices = 0;
	int countPoints = 0;
	section = 0;

	while(std::getline(objData, strInput)) {

		// for Vertices
		if(strInput[0] == 'v'  && strInput[1] == ' ' )
		{

			VerticesFilter(&strInput, &strOutput, &filename);
		}
		// for texture Vertices
		else if(strInput[0] == 'v' && strInput[1] == 't')
		{

			UVSFIlter(&strInput, &strOutput, &filename);
			countVertices++;
		}
		// for Points
		else if(strInput[0] == 'f' && strInput[1] == ' ')
		{

			PointsFilter(&strInput, &strOutput, &filename);
			countPoints++;
		}
	}

	std::stringstream ss;
	std::string tmp ="";

	ss << countPoints*3;
	ss >> tmp;
	strOutput = strOutput + "\n\nint points_count_" + filename + " = " + tmp + "; \n" ;
	
	ss.clear();
	tmp.clear();
	ss << countVertices;
	ss >> tmp;
	strOutput = strOutput + "\n\nint vertex_count_" + filename + " = " + tmp + "; \n" ;

	//std::cout << strOutput;
	tmp.clear();

	tmp = filename + ".h";

	std::ofstream outputFile(tmp);

	outputFile << "/*		This File Generated by Obj2C			**\n"
				"**		Created By Imandana	Rahimaswara			**\n"
				"**												**\n" 
				"**	=========================================	**/" + strOutput;

	objData.close();
	outputFile.close();

	std::cout << "DONE!!!";
	getchar();
	return 0;
}


void PointsFilter(std::string *dataIn, std::string *dataOut, std::string* filename)
{
	std::string tempComma = "";

	if( section == 2 ){
		section = 0;
		*dataOut = *dataOut + "\n\nint points_" + *filename +"[] = {\n\n};\n";
	}
	else
		tempComma = ",\n";

	for(int i = 3; i < dataIn->size(); i++)
	{
		if(dataIn->at(i) == ' ')
		{
			dataIn->at(i) = ',';
		}
	}
	// Calculte data
	std::string temp="";
	int tempInt[3] = {0,0,0};
	std::stringstream convert;

	int j=0;
	for(int i = 0; i < dataIn->size(); i++)
	{
		temp.clear();
		if(dataIn->at(i) == '/'){

			while(dataIn->at(++i) != '/')
			{
				if(dataIn->at(i) == ' ')
					break;

				temp = temp + dataIn->at(i);
			}
			convert.clear();

			convert << temp;
			convert >> tempInt[j];
			j++;
		}
	}
	//// Minus 1 for the data
	temp = "";
	std::string temp2 ="";
	for(int i =0 ; i< j; i++)
	{
		//std::cout<< tempInt[i] << " ";

		tempInt[i]--;
		convert.clear();
		temp2.clear();
		convert << tempInt[i];
		convert >> temp2;

		temp = temp +", " +temp2 ;
	}

	// 4 for remove the \n};\n
	// 2 to starts from 1 of "v 1000"
	*dataOut =  (*dataOut).substr(0, (*dataOut).size()-4) + tempComma + " " + temp.substr(1, temp.size()) + " " + "\n};\n";
}
void VerticesFilter(std::string *dataIn, std::string *dataOut, std::string* filename)
{
	std::string tempComma = "";

	if(section == 0){
		section =  1;
		*dataOut = *dataOut + "\n\nVECTOR vertices_" + *filename +"[] = {\n\n};\n";
	}
	else
		tempComma = ",\n";

	for(int i = 2; i < dataIn->size(); i++)
	{
		if(dataIn->at(i) == ' ')
		{
			dataIn->at(i) = ',';
		}
	}
	// 4 for remove the \n};\n
	// 2 to starts from 1 of "v 1000"
	*dataOut =  (*dataOut).substr(0, (*dataOut).size()-4) + tempComma + "{ " + (*dataIn).substr(2, (*dataIn).size()-2) + ", 1.00 } " + "\n};\n";
}
void UVSFIlter(std::string *dataIn, std::string *dataOut, std::string* filename)
{
	std::string tempComma = "";

	if( section == 1 ){
		section = 2;
		*dataOut = *dataOut + "\n\nVECTOR uvs_" + *filename +"[] = {\n\n};\n";
	}
	else
		tempComma = ",\n";

	for(int i = 3; i < dataIn->size(); i++)
	{
		if(dataIn->at(i) == ' ')
		{
			dataIn->at(i) = ',';
		}
	}
	// 4 for remove the \n};\n
	// 2 to starts from 1 of "v 1000"
	*dataOut =  (*dataOut).substr(0, (*dataOut).size()-4) + tempComma + "{ " + (*dataIn).substr(2, (*dataIn).size()-2) + ", 0.00, 0.00 } " + "\n};\n";
}
