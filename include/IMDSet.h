#pragma once
#ifndef _H_IMDSET_H_
#define _H_IMDSET_H_

#ifdef WIN32
#ifdef MDSet_EXPORTS
#define MDSET_API __declspec(dllexport)
#else
#define MDSET_API __declspec(dllimport)
#endif//
#else //LINUX
#define	MDSET_API 
#endif//

#if defined(__cplusplus)
extern "C" {
#endif//

MDSET_API void MDSet_init(char* xml, int xmlflag);
MDSET_API void MDSet_term();

#if defined(__cplusplus)
}

namespace MTP
{

class MDSET_API IMDSet
{
public:
    static IMDSet& Instance();

    virtual void init(char* xml, int xmlflag = 0) = 0;
    virtual void term() = 0;
};

}

#endif//

#endif//_H_IMDSET_H_