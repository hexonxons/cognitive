#ifndef __UTILS_H__
#define __UTILS_H__

#include <vector>

struct CTag
{
    CTag(short _Val1, char _Val2);
    CTag();
    short tag;
    char isClose;
};

inline int operator==(const CTag & _Left, const CTag& _Right)
{
    return (_Left.tag == _Right.tag && _Left.isClose == _Right.isClose);
}

inline int operator!=(const CTag & _Left, const CTag& _Right)
{
    return !(_Left == _Right);
}

//#########################################################################################

template <class _Ty1, class _Ty2> struct CPair
{
    CPair()
    {
        first = _Ty1();
        second = _Ty2();
    }
    CPair(_Ty1 _Val1, _Ty2 _Val2)
    {
        first = _Val1;
        second = _Val2;
    }
    _Ty1 first;
    _Ty2 second;
};

template<class _Ty1, class _Ty2> inline CPair<_Ty1, _Ty2> make_cpair(_Ty1 _Val1, _Ty2 _Val2)
{	
    return (CPair<_Ty1, _Ty2>(_Val1, _Val2));
}

template<class _Ty1, class _Ty2> inline int operator==(const CPair<_Ty1, _Ty2>& _Left,
                                                       const CPair<_Ty1, _Ty2>& _Right)
{
    return (_Left.first == _Right.first && _Left.second == _Right.second);
}

template<class _Ty1, class _Ty2> inline int operator!=(const CPair<_Ty1, _Ty2>& _Left,
                                                       const CPair<_Ty1, _Ty2>& _Right)
{
    return !(_Left == _Right);
}

//#########################################################################################

template <class _Ty1, class _Ty2, class _Ty3> struct CTriple
{
    CTriple()
    {
        first = _Ty1();
        second = _Ty2();
        third = _Ty3();
    }
    CTriple(_Ty1 _Val1, _Ty2 _Val2, _Ty3 _Val3)
    {
        first = _Val1;
        second = _Val2;
        third = _Val3;
    }
    _Ty1 first;
    _Ty2 second;
    _Ty3 third;
};

//#########################################################################################

int vStrCmp(const std::vector<CPair<CTag, CPair<int, int>>> &left,
            const std::vector<CPair<CTag, CPair<int, int>>> &right);

int vStrStr(const std::vector<CPair<CTag, CPair<int, int>>> &left,
            const std::vector<CPair<CTag, CPair<int, int>>> &right);

int vStrStr(const std::vector<CPair<CTag, CPair<int, int>>> &vStr1,
            const std::vector<CPair<CTag, CPair<int, int>>> &vStr2,
            int offset);

std::vector<CPair<CTag, CPair<int, int>>>::iterator pStrStr(std::vector<CPair<CTag, CPair<int, int>>> &vStr1,
                                                            std::vector<CPair<CTag, CPair<int, int>>> &vStr2,
                                                            int offset);
std::vector<CPair<CTag, CPair<int, int>>>::iterator pStrStr(std::vector<CPair<CTag, CPair<int, int>>> &vStr1,
                                                            std::vector<CPair<CTag, CPair<int, int>>> &vStr2);

#endif // __UTILS_H__