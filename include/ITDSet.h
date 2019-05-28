#pragma once
#ifndef _H_ITDSET_H_
#define _H_ITDSET_H_

#ifdef WIN32
#ifdef TDSet_EXPORTS
#define TDSET_API __declspec(dllexport)
#else
#define TDSET_API __declspec(dllimport)
#endif//
#else //LINUX
#define	TDSET_API 
#endif//

#if defined(__cplusplus)
extern "C" {
#endif//

TDSET_API void TDSet_init(char* xml, int xmlflag);
TDSET_API void TDSet_term();

#if defined(__cplusplus)
}

namespace MTP
{

class TDSET_API ITDSet
{
public:
    static ITDSet& Instance();

    virtual void init(char* xml, int xmlflag = 0) = 0;
    virtual void term() = 0;
};

}

#endif//

#endif//_H_ITDSET_H_