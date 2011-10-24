/*
 *  memalloc.h
 *  memory manager with alerts of banned functions.
 *  4 May 2011
 *  Hexonxonx
 */

#ifndef __MEMALLOC_H__
#define __MEMALLOC_H__

#include "banned.h"

// ������ �����
const int BLOCK_SIZE = 10;

template<class T> class CBasicDataBase
{
private:
	// ��������� ����� ������
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
		// ������� ������� � �����
		unsigned int m_posInBlock;
		// ���������� ������
		unsigned int m_blockCount;
		// ����� �������� �����
		unsigned int m_blockNum;
		// ���������� ��������� � �����
		unsigned int m_elemCount;

	};

	// �����������
	CBasicDataBase():
	   m_pData(NULL),
	   m_pBegin(0),
	   m_freeBlockNum(0),
	   m_elemCount(0),
	   m_blockCount(0)
	{
	}

	// ����������
	virtual ~CBasicDataBase()
	{
		if (m_pData != NULL)
		{
			Storage *pTmp;
			m_pData = m_pBegin;
			// ���� �� ���� ������ � ������� ����������
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
            // ���� �� ���� ������ � ������� ����������
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
	// ������� ���������� �������� � ����
	T* Add(T& elem)
	{
		// ���� ���������� ��������� ������ � ������ ����� == 0
		if(m_freeBlockNum == 0)
		{
			// ���� ��� ������ ����, �� �������� �� ���� ������
			if (m_blockCount == 0)
			{
				m_pData = new Storage;
				m_pBegin = m_pData;
			}
			else
			{
				// ���� �� ������ ����, �� �������� ������ �� ��������� ����
				m_pData->pNext = new Storage;
				// ������������ ��������� �� ����� ����
				m_pData = m_pData->pNext;

			}		
			// �������� ������ � ����� �� BLOCK_SIZE ���������
			m_pData->pData = new T[BLOCK_SIZE];
			m_pData->pNext = NULL;
			// �������� ��� �������� ��� ���������
			m_freeBlockNum = BLOCK_SIZE;
			// ����������� ����� ������ 
			++m_blockCount;

		}
		// ���������
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
	// ��������� �� ������� ����
	Storage *m_pData;
	// ��������� �� ������
	Storage *m_pBegin;
	// ���������� ��������� ���� � �����
	unsigned int m_freeBlockNum;
	// ���������� ������
	unsigned int m_blockCount;
	// ���������� ��������� � �����
	unsigned int m_elemCount;
	// ��������� ������
	unsigned int m_lastError;
};

#endif  // __MEMALLOC_H__