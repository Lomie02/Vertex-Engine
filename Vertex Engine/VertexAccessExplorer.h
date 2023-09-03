#pragma once
#include <Windows.h>
#include <shellapi.h>
#include <iostream>
class VertexAccessExplorer
{
public:
	/// <summary>
	/// Opens an given URL in the default browser being used on the current OS.
	/// </summary>
	/// <param name="Link to open"></param>
	static bool OpenURL(const wchar_t* path); 
	/// <summary>
	/// Opens any given program name or file path.
	/// </summary>
	/// <param name="Name of software or file path to open."></param>
	/// <returns></returns>
	static bool OpenProgram(std::string _name);
	/// <summary>
	/// Opens Google into the default web browser.
	/// </summary>
	static void OpenGoogle();
	/// <summary>
	/// Opens Bing into the default web browser.
	/// </summary>
	static void OpenBing();

private:
	VertexAccessExplorer();
};

