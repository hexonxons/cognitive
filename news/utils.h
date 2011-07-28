/**
 * \file    utils.h
 *
 * \brief   Вспомогательные функции и структуры данных. Объявления.
 */

#ifndef __UTILS_H__
#define __UTILS_H__

#include <vector>

/**
 * \struct  CTag
 *
 * \brief   Стректура тега. 
 * 			tag - код тега. 
 * 			isClose - закрывающий тег или открывающий.
 *
 * \author  Alexander
 * \date    7/28/2011
 */

struct CTag
{
    /**
     * \fn  CTag(short _Tag, char _IsClose);
     *
     * \brief   Конструктор.
     *
     * \author  Alexander
     * \date    7/28/2011
     *
     * \param   _Tag        Значение tag.
     * \param   _IsClose    Значение isClose.
     */

    CTag(short _Tag, char _IsClose);

    /**
     * \fn  CTag();
     *
     * \brief   Конструктор по умолчанию.
     * 			Инициализирует tag и isClose нулями.
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
 * \brief   Оператор ==.
 *
 * \author  Alexander
 * \date    7/28/2011
 *
 * \param   _Left   Левый аргумент.
 * \param   _Right  Правый аргумент.
 *
 * \return  1, если _Left == _Right.
 */

inline int operator==(const CTag & _Left, const CTag& _Right)
{
    return (_Left.tag == _Right.tag && _Left.isClose == _Right.isClose);
}

/**
 * \fn  inline int operator!=(const CTag & _Left, const CTag& _Right)
 *
 * \brief   Оператор !=.
 *
 * \author  Alexander
 * \date    7/28/2011
 *
 * \param   _Left   Левый аргумент.
 * \param   _Right  Правый аргумент.
 * 					
 * \return  1, если _Left != _Right.
 */

inline int operator!=(const CTag & _Left, const CTag& _Right)
{
    return !(_Left == _Right);
}

//#########################################################################################

/**
 * \struct  CPair
 *
 * \brief   Структура пары.. 
 *
 * \author  Alexander
 * \date    7/28/2011
 */

template <class _Ty1, class _Ty2> struct CPair
{
    /**
     * \fn  CPair::CPair()
     *
     * \brief   Конструктор по умолчанию.
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
     * \brief   Конструктор.
     *
     * \author  Alexander
     * \date    7/28/2011
     *
     * \param   _Val1   Значение first.
     * \param   _Val2   Значение second.
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
 * \brief   Создание cPair.
 *
 * \author  Alexander
 * \date    7/28/2011
 *
 * \param   _Val1   Значение first.
 * \param   _Val2   Значение second.
 *
 * \return  Объект CPair инициализированный значениями _Val1 и _Val2.
 */

template<class _Ty1, class _Ty2> inline CPair<_Ty1, _Ty2> make_cpair(_Ty1 _Val1, _Ty2 _Val2)
{	
    return (CPair<_Ty1, _Ty2>(_Val1, _Val2));
}

/**
 * \fn  template<class _Ty1, class _Ty2> inline int operator==(const CPair<_Ty1, _Ty2>& _Left,
 *      const CPair<_Ty1, _Ty2>& _Right)
 *
 * \brief   Оператор ==.
 *
 * \author  Alexander
 * \date    7/28/2011
 *
 * \param   _Left   Левый аргумент.
 * \param   _Right  Правый аргумент.
 *
 * \return  1, если _Left == _Right.
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
 * \brief   Оператор !=..=
 *
 * \author  Alexander
 * \date    7/28/2011
 *
 * \param   _Left   Левый аргумент.
 * \param   _Right  Правый аргумент.
 * 					
 * \return  1, если _Left != _Right.					
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
 * \brief   Структура из 3х элементов. 
 *
 * \author  Alexander
 * \date    7/28/2011
 */

template <class _Ty1, class _Ty2, class _Ty3> struct CTriple
{
    /**
     * \fn  CTriple::CTriple()
     *
     * \brief   Конструктор по умолчанию.
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
     * \brief   Конструктор.
     *
     * \author  Alexander
     * \date    7/28/2011
     *
     * \param   _Val1   Значение first.
     * \param   _Val2   Значение second.
     * \param   _Val3   Значение third.
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
 * \brief   Функция производит поиск vStr2 в vStr1.
 *
 * \author  Alexander
 * \date    7/28/2011
 *
 * \param   vStr1   Строка, в которой ищем.
 * \param   vStr2   Строка, которую ищем.
 *
 * \return  1, если в vStr1 содержится vStr2
 * 			0, иначе.
 */

int vIsSubstr(const std::vector<CPair<CTag, CPair<int, int>>> &vStr1,
              const std::vector<CPair<CTag, CPair<int, int>>> &vStr2);

/**
 * \fn  int vStrStr(const std::vector<CPair<CTag, CPair<int, int>>> &vStr1,
 *      const std::vector<CPair<CTag, CPair<int, int>>> &vStr2, int offset);
 *
 * \brief   Функция производит поиск позиции, с которой vStr2 совпадает с vStr1,
 * 			начиная с позиции offset в vStr1.
 *
 * \author  Alexander
 * \date    7/28/2011
 *
 * \param   vStr1   Строка, в которой ищем.
 * \param   vStr2   Строка, которую ищем.
 * \param   offset  Сдвиг.
 *
 * \return  Позицию, с которой начинается совпадение.
 * 			0, если совпадений нет.
 */

int vStrStr(const std::vector<CPair<CTag, CPair<int, int>>> &vStr1,
            const std::vector<CPair<CTag, CPair<int, int>>> &vStr2,
            int offset);

/**
 * \fn  std::vector<CPair<CTag, CPair<int, int>>>::iterator pStrStr(std::vector<CPair<CTag, CPair<int, int>>> &vStr1,
 *      std::vector<CPair<CTag, CPair<int, int>>> &vStr2, int offset);
 *
 * \brief   Функция производит поиск позиции, с которой vStr2 совпадает с vStr1,
 * 			начиная с позиции offset в vStr1.
 *
 * \author  Alexander
 * \date    7/28/2011
 *
 * \param [in,out]  vStr1   Строка, в которой ищем.
 * \param [in,out]  vStr2   Строка, которую ищем.
 * \param   offset          Сдвиг.
 *
 * \return  Итератор на начало совпадения.
 * 			Итератор на vStr1.end(), если совпадений не было.
 */

std::vector<CPair<CTag, CPair<int, int>>>::iterator pStrStr(std::vector<CPair<CTag, CPair<int, int>>> &vStr1,
                                                            std::vector<CPair<CTag, CPair<int, int>>> &vStr2,
                                                            int offset);

/**
 * \fn  std::vector<CPair<CTag, CPair<int, int>>>::iterator pStrStr(std::vector<CPair<CTag, CPair<int, int>>> &vStr1,
 *      std::vector<CPair<CTag, CPair<int, int>>> &vStr2);
 *
 * \brief   Функция производит поиск позиции, с которой vStr2 совпадает с vStr1.
 *
 * \author  Alexander
 * \date    7/28/2011
 *
 * \param [in,out]  vStr1   Строка, в которой ищем.
 * \param [in,out]  vStr2   Строка, которую ищем.
 *
 * \return  Итератор на начало совпадения.
 * 			Итератор на vStr1.end(), если совпадений не было.
 */

std::vector<CPair<CTag, CPair<int, int>>>::iterator pStrStr(std::vector<CPair<CTag, CPair<int, int>>> &vStr1,
                                                            std::vector<CPair<CTag, CPair<int, int>>> &vStr2);

#endif // __UTILS_H__