/**
 * \file    utils.h
 *
 * \brief   ��������������� ������� � ��������� ������. ����������.
 */

#ifndef __UTILS_H__
#define __UTILS_H__

#include <vector>

/**
 * \struct  CTag
 *
 * \brief   ��������� ����. 
 * 			tag - ��� ����. 
 * 			isClose - ����������� ��� ��� �����������.
 *
 * \author  Alexander
 * \date    7/28/2011
 */

struct CTag
{
    /**
     * \fn  CTag(short _Tag, char _IsClose);
     *
     * \brief   �����������.
     *
     * \author  Alexander
     * \date    7/28/2011
     *
     * \param   _Tag        �������� tag.
     * \param   _IsClose    �������� isClose.
     */

    CTag(short _Tag, char _IsClose);

    /**
     * \fn  CTag();
     *
     * \brief   ����������� �� ���������.
     * 			�������������� tag � isClose ������.
     *
     * \author  Alexander
     * \date    7/28/2011
     */

    CTag();
    short tag;
    char isClose;
};

/**
 * \fn  inline int operator==(const CTag & _Left, const CTag& _Right)
 *
 * \brief   �������� ==.
 *
 * \author  Alexander
 * \date    7/28/2011
 *
 * \param   _Left   ����� ��������.
 * \param   _Right  ������ ��������.
 *
 * \return  1, ���� _Left == _Right.
 */

inline int operator==(const CTag & _Left, const CTag& _Right)
{
    return (_Left.tag == _Right.tag && _Left.isClose == _Right.isClose);
}

/**
 * \fn  inline int operator!=(const CTag & _Left, const CTag& _Right)
 *
 * \brief   �������� !=.
 *
 * \author  Alexander
 * \date    7/28/2011
 *
 * \param   _Left   ����� ��������.
 * \param   _Right  ������ ��������.
 * 					
 * \return  1, ���� _Left != _Right.
 */

inline int operator!=(const CTag & _Left, const CTag& _Right)
{
    return !(_Left == _Right);
}

//#########################################################################################

/**
 * \struct  CPair
 *
 * \brief   ��������� ����.. 
 *
 * \author  Alexander
 * \date    7/28/2011
 */

template <class _Ty1, class _Ty2> struct CPair
{
    /**
     * \fn  CPair::CPair()
     *
     * \brief   ����������� �� ���������.
     *
     * \author  Alexander
     * \date    7/28/2011
     */

    CPair()
    {
        first = _Ty1();
        second = _Ty2();
    }

    /**
     * \fn  CPair::CPair(_Ty1 _Val1, _Ty2 _Val2)
     *
     * \brief   �����������.
     *
     * \author  Alexander
     * \date    7/28/2011
     *
     * \param   _Val1   �������� first.
     * \param   _Val2   �������� second.
     */

    CPair(_Ty1 _Val1, _Ty2 _Val2)
    {
        first = _Val1;
        second = _Val2;
    }

    _Ty1 first;
    _Ty2 second;
};

/**
 * \fn  template<class _Ty1, class _Ty2> inline CPair<_Ty1, _Ty2> make_cpair(_Ty1 _Val1,
 *      _Ty2 _Val2)
 *
 * \brief   �������� cPair.
 *
 * \author  Alexander
 * \date    7/28/2011
 *
 * \param   _Val1   �������� first.
 * \param   _Val2   �������� second.
 *
 * \return  ������ CPair ������������������ ���������� _Val1 � _Val2.
 */

template<class _Ty1, class _Ty2> inline CPair<_Ty1, _Ty2> make_cpair(_Ty1 _Val1, _Ty2 _Val2)
{	
    return (CPair<_Ty1, _Ty2>(_Val1, _Val2));
}

/**
 * \fn  template<class _Ty1, class _Ty2> inline int operator==(const CPair<_Ty1, _Ty2>& _Left,
 *      const CPair<_Ty1, _Ty2>& _Right)
 *
 * \brief   �������� ==.
 *
 * \author  Alexander
 * \date    7/28/2011
 *
 * \param   _Left   ����� ��������.
 * \param   _Right  ������ ��������.
 *
 * \return  1, ���� _Left == _Right.
 */

template<class _Ty1, class _Ty2> inline int operator==(const CPair<_Ty1, _Ty2>& _Left,
                                                       const CPair<_Ty1, _Ty2>& _Right)
{
    return (_Left.first == _Right.first && _Left.second == _Right.second);
}

/**
 * \fn  template<class _Ty1, class _Ty2> inline int operator!=(const CPair<_Ty1, _Ty2>& _Left,
 *      const CPair<_Ty1, _Ty2>& _Right)
 *
 * \brief   �������� !=..=
 *
 * \author  Alexander
 * \date    7/28/2011
 *
 * \param   _Left   ����� ��������.
 * \param   _Right  ������ ��������.
 * 					
 * \return  1, ���� _Left != _Right.					
 */

template<class _Ty1, class _Ty2> inline int operator!=(const CPair<_Ty1, _Ty2>& _Left,
                                                       const CPair<_Ty1, _Ty2>& _Right)
{
    return !(_Left == _Right);
}

//#########################################################################################

/**
 * \struct  CTriple
 *
 * \brief   ��������� �� 3� ���������. 
 *
 * \author  Alexander
 * \date    7/28/2011
 */

template <class _Ty1, class _Ty2, class _Ty3> struct CTriple
{
    /**
     * \fn  CTriple::CTriple()
     *
     * \brief   ����������� �� ���������.
     *
     * \author  Alexander
     * \date    7/28/2011
     */

    CTriple()
    {
        first = _Ty1();
        second = _Ty2();
        third = _Ty3();
    }

    /**
     * \fn  CTriple::CTriple(_Ty1 _Val1, _Ty2 _Val2, _Ty3 _Val3)
     *
     * \brief   �����������.
     *
     * \author  Alexander
     * \date    7/28/2011
     *
     * \param   _Val1   �������� first.
     * \param   _Val2   �������� second.
     * \param   _Val3   �������� third.
     */

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

/**
 * \fn  int vIsSubstr(const std::vector<CPair<CTag, CPair<int, int>>> &vStr1,
 *      const std::vector<CPair<CTag, CPair<int, int>>> &vStr1);
 *
 * \brief   ������� ���������� ����� vStr2 � vStr1.
 *
 * \author  Alexander
 * \date    7/28/2011
 *
 * \param   vStr1   ������, � ������� ����.
 * \param   vStr2   ������, ������� ����.
 *
 * \return  1, ���� � vStr1 ���������� vStr2
 * 			0, �����.
 */

int vIsSubstr(const std::vector<CPair<CTag, CPair<int, int>>> &vStr1,
              const std::vector<CPair<CTag, CPair<int, int>>> &vStr2);

/**
 * \fn  int vStrStr(const std::vector<CPair<CTag, CPair<int, int>>> &vStr1,
 *      const std::vector<CPair<CTag, CPair<int, int>>> &vStr2, int offset);
 *
 * \brief   ������� ���������� ����� �������, � ������� vStr2 ��������� � vStr1,
 * 			������� � ������� offset � vStr1.
 *
 * \author  Alexander
 * \date    7/28/2011
 *
 * \param   vStr1   ������, � ������� ����.
 * \param   vStr2   ������, ������� ����.
 * \param   offset  �����.
 *
 * \return  �������, � ������� ���������� ����������.
 * 			0, ���� ���������� ���.
 */

int vStrStr(const std::vector<CPair<CTag, CPair<int, int>>> &vStr1,
            const std::vector<CPair<CTag, CPair<int, int>>> &vStr2,
            int offset);

/**
 * \fn  std::vector<CPair<CTag, CPair<int, int>>>::iterator pStrStr(std::vector<CPair<CTag, CPair<int, int>>> &vStr1,
 *      std::vector<CPair<CTag, CPair<int, int>>> &vStr2, int offset);
 *
 * \brief   ������� ���������� ����� �������, � ������� vStr2 ��������� � vStr1,
 * 			������� � ������� offset � vStr1.
 *
 * \author  Alexander
 * \date    7/28/2011
 *
 * \param [in,out]  vStr1   ������, � ������� ����.
 * \param [in,out]  vStr2   ������, ������� ����.
 * \param   offset          �����.
 *
 * \return  �������� �� ������ ����������.
 * 			�������� �� vStr1.end(), ���� ���������� �� ����.
 */

std::vector<CPair<CTag, CPair<int, int>>>::iterator pStrStr(std::vector<CPair<CTag, CPair<int, int>>> &vStr1,
                                                            std::vector<CPair<CTag, CPair<int, int>>> &vStr2,
                                                            int offset);

/**
 * \fn  std::vector<CPair<CTag, CPair<int, int>>>::iterator pStrStr(std::vector<CPair<CTag, CPair<int, int>>> &vStr1,
 *      std::vector<CPair<CTag, CPair<int, int>>> &vStr2);
 *
 * \brief   ������� ���������� ����� �������, � ������� vStr2 ��������� � vStr1.
 *
 * \author  Alexander
 * \date    7/28/2011
 *
 * \param [in,out]  vStr1   ������, � ������� ����.
 * \param [in,out]  vStr2   ������, ������� ����.
 *
 * \return  �������� �� ������ ����������.
 * 			�������� �� vStr1.end(), ���� ���������� �� ����.
 */

std::vector<CPair<CTag, CPair<int, int>>>::iterator pStrStr(std::vector<CPair<CTag, CPair<int, int>>> &vStr1,
                                                            std::vector<CPair<CTag, CPair<int, int>>> &vStr2);

#endif // __UTILS_H__