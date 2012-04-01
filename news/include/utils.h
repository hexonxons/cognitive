/**
 * \file    utils.h
 *
 * \brief   ¬спомогательные функции и структуры данных. ќбъ€влени€.
 */

#ifndef __UTILS_H__
#define __UTILS_H__

#include <vector>

/**
 * \struct  CTagDescription
 *
 * \brief   —труктура закодированного тега
 *
 * \author  Alexander
 * \date    11/17/2011
 */
struct CTagDescription
{
    // сам тег
    std::string tag;
    // начала тега в строке
    int nTagBegin;
    // конец тега в строке
    int nTagEnd;
    // хеш тега
    int nTagCode;
    // закрывающий тег или нет
    bool bIsClose;
 
    CTagDescription();
    void Clear();
};

struct CTagRange
{
    int CloseBegin;
    int CloseEnd;
    // начало последовательности тегов в тексте
    int Begin;
    // конец последовательности
    int End;

    // вс€ строка между begin и end
    std::string TagString;
    // отношение всей длины текущей последовательности ко всей длине html странички
    double PercToHtml;
    // отношение видимой части последовательности к видимой части всей html странички
    double PercToVisibleHtml;

    long InnerIntersection;
    long DistanceToNextRange;

    
    std::string CloseTagString;
    double ClosePercToHtml;
    double ClosePercToVisibleHtml;

    CTagRange();
};

struct CTagSequence 
{
    std::vector<CTagDescription> tag;
    std::vector<CTagDescription> closedTag;
    std::vector<CTagRange> tagRange;
    // сумма пересечений этой последоват%ельности со всеми другими
    int innerIntersect;
    double dispersion;
    // отношение суммы всех длин последовательностей ко всей длине html странички
    double percToHtml;
    // отношение видимой части последовательности к видимой части всей html странички
    double percToVisibleHtml;
    // сумма рассто€ний между блоками
    int InnerDistance;
    // сумма разниц между дистанци€ми дл€ каждых блоков и средней дистанцией дл€ всей последовательности
    double InnerDistanceDiffSum;
    // 
    int InternalIntersection;
    // отношение суммы всех длин последовательностей ко всей длине html странички
    double ClosePercToHtml;
    // отношение видимой части последовательности к видимой части всей html странички
    double ClosePercToVisibleHtml;
    int CloseLen;
    CTagSequence();
};

//#########################################################################################

int vStrCmp(const std::vector<CTagDescription> &left,
            const std::vector<CTagDescription> &right);

/**
 * \fn  int vIsSubstr(const std::vector<CPair<CTag, CPair<int, int>>> &vStr1,
 *      const std::vector<CPair<CTag, CPair<int, int>>> &vStr2);
 *
 * \brief   ‘ункци€ производит поиск vStr2 в vStr1.
 *
 * \author  Alexander
 * \date    7/28/2011
 *
 * \param   vStr1   —трока, в которой ищем.
 * \param   vStr2   —трока, которую ищем.
 *
 * \return  1, если в vStr1 содержитс€ vStr2
 * 			0, иначе.
 */

int vIsSubstr(const std::vector<CTagDescription> &vStr1,
              const std::vector<CTagDescription> &vStr2);

/**
 * \fn  std::vector<CPair<CTag, CPair<int, int>>>::iterator pStrStr(std::vector<CPair<CTag, CPair<int, int>>> &vStr1,
 *      std::vector<CPair<CTag, CPair<int, int>>> &vStr2, int offset);
 *
 * \brief   ‘ункци€ производит поиск позиции, с которой vStr2 совпадает с vStr1,
 * 			начина€ с позиции offset в vStr1.
 *
 * \author  Alexander
 * \date    7/28/2011
 *
 * \param [in,out]  vStr1   —трока, в которой ищем.
 * \param [in,out]  vStr2   —трока, которую ищем.
 * \param   offset          —двиг.
 *
 * \return  »тератор на начало совпадени€.
 * 			»тератор на vStr1.end(), если совпадений не было.
 */

std::vector<CTagDescription>::iterator pStrStr(std::vector<CTagDescription> &vStr1,
                                               std::vector<CTagDescription> &vStr2,
                                               int offset);

/**
 * \fn  std::vector<CPair<CTag, CPair<int, int>>>::iterator pStrStr(std::vector<CPair<CTag, CPair<int, int>>> &vStr1,
 *      std::vector<CPair<CTag, CPair<int, int>>> &vStr2);
 *
 * \brief   ‘ункци€ производит поиск позиции, с которой vStr2 совпадает с vStr1.
 *
 * \author  Alexander
 * \date    7/28/2011
 *
 * \param [in,out]  vStr1   —трока, в которой ищем.
 * \param [in,out]  vStr2   —трока, которую ищем.
 *
 * \return  »тератор на начало совпадени€.
 * 			»тератор на vStr1.end(), если совпадений не было.
 */

std::vector<CTagDescription>::iterator pStrStr(std::vector<CTagDescription> &vStr1,
                                               std::vector<CTagDescription> &vStr2);

char *GetPageSource(const char *URL);

#endif // __UTILS_H__