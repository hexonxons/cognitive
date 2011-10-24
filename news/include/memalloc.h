/*
 *  memalloc.h
 *  memory manager with alerts of banned functions.
 *  4 May 2011
 *  Hexonxonx
 */

#ifndef __MEMALLOC_H__
#define __MEMALLOC_H__

#include "banned.h"

// размер блока
const int BLOCK_SIZE = 10;

template<class T> class CBasicDataBase
{
private:
	// структура блока данных
	struct Storage
	{
		T		*pData;
		Storage *pNext;
	};

public:
	class Iterator
	{
	public:
		Iterator(CBasicDataBase *elem)
		{
			m_pData = elem->m_pBegin;
			m_posInBlock = 0;
			m_blockNum = 0;
			m_elemCount = elem->m_elemCount;
		}
        virtual ~Iterator()
        {
            m_pData = NULL;
            m_posInBlock = 0;
            m_blockNum = 0;
            m_blockCount = 0;
            m_elemCount = 0;
        }

		T *Next()
		{
			++m_posInBlock;
			if (m_posInBlock == BLOCK_SIZE && m_pData->pNext != NULL)
			{
				m_posInBlock = 0;
				m_pData = m_pData->pNext;
				++m_blockNum;
			}
			return m_pData->pData + m_posInBlock;
		}

		T *Current()
		{
			return m_pData->pData + m_posInBlock;
		}

		bool isValid()
		{
			return(m_blockNum * BLOCK_SIZE + m_posInBlock != m_elemCount);
		}
	private:
		Iterator():
			m_pData(NULL),
			m_posInBlock(0),
			m_blockNum(0),
			m_elemCount(0)

		{
		}
		Storage *m_pData;
		// текущая позиция в блоке
		unsigned int m_posInBlock;
		// количество блоков
		unsigned int m_blockCount;
		// номер текущего блока
		unsigned int m_blockNum;
		// количество элементов в листе
		unsigned int m_elemCount;

	};

	// конструктор
	CBasicDataBase():
	   m_pData(NULL),
	   m_pBegin(0),
	   m_freeBlockNum(0),
	   m_elemCount(0),
	   m_blockCount(0)
	{
	}

	// деструктор
	virtual ~CBasicDataBase()
	{
		if (m_pData != NULL)
		{
			Storage *pTmp;
			m_pData = m_pBegin;
			// идем на блок вперед и удаляем предыдущий
			while(m_pData->pNext)
			{
				pTmp = m_pData;
				m_pData = m_pData->pNext;
				if(pTmp->pData)
				{
					delete[] pTmp->pData;
					pTmp->pData = NULL;
				}
				delete pTmp;
				pTmp = NULL;
			}
			delete[] m_pData->pData;
			m_pData->pData = NULL;
			delete m_pData;
			m_pData = NULL;
			m_pBegin = NULL;
		}
	}

    //for tester
    int destroy()
    {
        if (m_pData != NULL)
        {
            Storage *pTmp;
            m_pData = m_pBegin;
            // идем на блок вперед и удаляем предыдущий
            while(m_pData->pNext)
            {
                pTmp = m_pData;
                m_pData = m_pData->pNext;
                if(pTmp->pData)
                {
                    delete[] pTmp->pData;
                    pTmp->pData = NULL;
                }
                delete pTmp;
                pTmp = NULL;
            }
            delete[] m_pData->pData;
            m_pData->pData = NULL;
            delete m_pData;
            m_pData = NULL;
            m_pBegin = NULL;
        }
        m_freeBlockNum = 0;
        m_blockCount = 0;
        m_elemCount = 0;
        return 0;
    }
	// функция добавления элемента в лист
	T* Add(T& elem)
	{
		// если количество свободных яйчеек в данном блоке == 0
		if(m_freeBlockNum == 0)
		{
			// если это первый блок, то выделяем на него память
			if (m_blockCount == 0)
			{
				m_pData = new Storage;
				m_pBegin = m_pData;
			}
			else
			{
				// если не первый блок, то выделяем память на следующий блок
				m_pData->pNext = new Storage;
				// переставляем указатель на новый блок
				m_pData = m_pData->pNext;

			}		
			// выделяем память в блоке на BLOCK_SIZE элементов
			m_pData->pData = new T[BLOCK_SIZE];
			m_pData->pNext = NULL;
			// помечаем все элементы как свободные
			m_freeBlockNum = BLOCK_SIZE;
			// увеличиваем число блоков 
			++m_blockCount;

		}
		// добавляем
		m_pData->pData[BLOCK_SIZE - m_freeBlockNum] = elem;
		--m_freeBlockNum;
		++m_elemCount;
		return m_pData->pData + BLOCK_SIZE - m_freeBlockNum - 1;
	}

	Iterator Begin()
	{
		return Iterator(this);
	}

private:
	// указатель на текущий блок
	Storage *m_pData;
	// указатель на начало
	Storage *m_pBegin;
	// количество свободных мест в блоке
	unsigned int m_freeBlockNum;
	// количество блоков
	unsigned int m_blockCount;
	// количество элементов в листе
	unsigned int m_elemCount;
	// последняя ошибка
	unsigned int m_lastError;
};

#endif  // __MEMALLOC_H__