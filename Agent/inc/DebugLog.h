#pragma once
#ifndef _DEBUGLOG_H_
#define _DEBUGLOG_H_

#include <iostream>
#include <windows.h>

#ifdef DEBUG
#define DEBUG_LOG(string) std::cout << string << GetLastError() << std::endl
#else
#define DEBUG_LOG(string)
#endif

#endif