/** 
 * @file   testCatchFox.cpp
 * @author smglaksn
 * @date   2014/04/27 01:26:50
 */

/*
  题目描述:
  在立方体2*2*2的8间房子里，有一只狐狸，它每天晚上换到相邻的房子里（不能对角线移动），
  而你是猎人，每天早上只能去3次，一次只能进一个房间，并立刻返回，
  问怎么进房子才能抓住狐狸，最少能保证几天抓住？
 */
 
#include <algorithm>
#include <vector>
#include <string>
#include <stdlib.h>
#include <stdio.h>

#define ARRAY_SIZE(ARRAY) ((sizeof(ARRAY) / sizeof(ARRAY[0])))
struct Cell
{
    enum
    {
        NUM_OF_NEXT_CELL = 3,
    };

    int number;
    int nextCellVector[NUM_OF_NEXT_CELL];
};

class Cube
{
public:
    enum
    {
        MAX_CELL_NUM = 8,
    };
    
    static Cube * getInstance()
    {
        static Cube cube;
        return &cube;
    }

    void printValue()
    {
        for (int i = 0; i < MAX_CELL_NUM; i++)
        {
            const Cell * tmp = &CellArray[i];
            fprintf(stderr, "index [%d] : next [%d] [%d] [%d]\n", tmp->number, 
                tmp->nextCellVector[0],
                tmp->nextCellVector[1],
                tmp->nextCellVector[2]);
        }
    }

    int getEqualArrayIndex(std::vector<int> & compareData)
        {
            int nRet = 0;
            do
            {
                if (compareData.size() != Cell::NUM_OF_NEXT_CELL)
                    break;

                std::vector<int> tmpData(compareData);
                std::sort(tmpData.begin(), tmpData.end());

                for (int i = 0; i < MAX_CELL_NUM; i++)
                {
                    if (CellArray[i].nextCellVector[0] == tmpData[0]
                        && CellArray[i].nextCellVector[1] == tmpData[1]
                        && CellArray[i].nextCellVector[2] == tmpData[2])
                    {
                        nRet = i+1;
                        break;
                    }
                }
            } while (0);
            return nRet;
        }

    bool isAtIndexArrayValue(int index, int value)
        {
            bool bRet = false;
            if (isValidIndex(index))
            {
                int i = index - 1;
                if (CellArray[i].nextCellVector[0] == value
                    || CellArray[i].nextCellVector[1] == value
                    || CellArray[i].nextCellVector[2] == value)
                {
                    bRet = true;
                }
            }
            return bRet;
        }
    void getIndexCanContainValue(std::vector<int> & inputData, std::vector<int> & ouputData)
        {
            std::vector<int> tmp;
            for (int i = 0; i < inputData.size(); i++)
            {
                if (isValidIndex(inputData[i]))
                {
                    int n = inputData[i] - 1;
                    tmp.push_back(CellArray[n].nextCellVector[0]);
                    tmp.push_back(CellArray[n].nextCellVector[1]);
                    tmp.push_back(CellArray[n].nextCellVector[2]);
                }
            }
            std::sort(tmp.begin(), tmp.end());
            tmp.erase(std::unique(tmp.begin(), tmp.end()), tmp.end());
            ouputData = tmp;
        }
    
    bool isValidIndex(int index)
        {
            return (index > 0 && index <= MAX_CELL_NUM);
        }
    void getIndexValue(int index, std::vector<int> & outData)
        {
            outData.clear();
            if (index > 0 && index <= MAX_CELL_NUM)
            {
                for (int i = 0; i < Cell::NUM_OF_NEXT_CELL; i++)
                    outData.push_back(CellArray[index-1].nextCellVector[i]);
            }
        }

private:

    static const Cell CellArray[];
};

const Cell Cube::CellArray[] = {
    {1, {2,4,5}},
    {2, {1,3,6}},
    {3, {2,4,7}},
    {4, {1,3,8}},
    {5, {1,6,8}},
    {6, {2,5,8}},
    {7, {3,6,8}},
    {8, {4,5,7}},
};

class SolverData
{
public:
    bool isSolve()
        {
            return (m_arrayNextStatus.size() == 0);
        }
    
    bool isReachLimit()
        {
            return (m_arraySolveSequence.size() >= E_MAX_SOLVE_SEQUENCE);
        }

    bool isAGoodStratry()
        {
            if (m_arrayNextStatus.size() == 0
                || m_arrayCurStatus.size() == 0)
                return false;
            
            if (m_arrayNextStatus.size() < m_arrayCurStatus.size())
                return true;
            if (m_arrayNextStatus.size() == m_arrayCurStatus.size())
            {
                return (!std::equal(m_arrayCurStatus.begin(), m_arrayCurStatus.end(),
                                    m_arrayNextStatus.begin()));
                
            }
            else
                return false;
        }
    enum {
        E_MAX_SOLVE_SEQUENCE = 4,
        E_SOLVE_SEQUENCE = 0,
        E_CUR_STATUS,
        E_NEXT_STATUS,
    };
    
    void setStatusValue(int status, std::vector<int> & vec)
        {
            std::vector<int> * tmpData = getStatusArray(status);
            if (tmpData)
                *tmpData = vec;
        }
    void setStatusValue(int status, int array[], int size)
        {
            std::vector<int> * tmpData = getStatusArray(status);
            if (tmpData)
            {
                tmpData->clear();
                for (int i = 0; i < size; i++)
                    tmpData->push_back(array[i]);
            }
        }

    void addValue(int status, int value)
        {
            std::vector<int> * tmpData = getStatusArray(status);
            if (tmpData)
                tmpData->push_back(value);
        }
    void clearArray(int status)
        {
            std::vector<int> * tmpData = getStatusArray(status);
            if (tmpData)
                tmpData->clear();
        }
    void copyStatus(int statusSrc, int statusDest)
        {
            std::vector<int> * src = getStatusArray(statusSrc);
            std::vector<int> * dest = getStatusArray(statusDest);
            if (src && dest)
            {
                *dest = *src;
            }
        }
    void printArray(int status)
        {
            std::vector<int> * tmpData = getStatusArray(status);
            if (tmpData)
            {
                fprintf(stderr, "array[%s]:", getStatusName(status).c_str());
                for (int i = 0; i < tmpData->size(); i++)
                {
                    fprintf(stderr, "[%d]", tmpData->at(i));
                }
                fprintf(stderr, "\n");
            }
        }
    void printAllStatus()
        {
            fprintf(stderr, "=========[status info]=========\n");
            printArray(E_SOLVE_SEQUENCE);
            printArray(E_CUR_STATUS);
            printArray(E_NEXT_STATUS);
        }
    std::string getStatusName(int status)
        {
            std::string str;
            switch (status)
            {
                case E_SOLVE_SEQUENCE:
                    str = "solve_seq";
                    break;
                case E_CUR_STATUS:
                    str = "cur_status";
                    break;
                case E_NEXT_STATUS:
                    str = "next_status";
                    break;
                default:
                    str = "error";
                    break;
            }
            return str;
        }
    void calcNextStatus(int index)
        {
            if (m_arrayCurStatus.size() > 0 && Cube::getInstance()->isValidIndex(index))
            {
                std::vector<int> tmpData(m_arrayCurStatus);
                std::vector<int>::iterator it = tmpData.begin();
                for (; it != tmpData.end();)
                {
                    int value = *it;
                    if (Cube::getInstance()->isAtIndexArrayValue(index, value))
                        it = tmpData.erase(it);
                    else
                        it++;
                }
                /*
                fprintf(stderr, "array [tmp]:");

                for (int i = 0; i < tmpData.size(); i++)
                {
                    fprintf(stderr, " [%d] ", tmpData.at(i));
                }
                fprintf(stderr, "\n");
                */
                Cube::getInstance()->getIndexCanContainValue(tmpData, m_arrayNextStatus);
                m_arraySolveSequence.push_back(index);
            }
        }
    
    
    std::vector<int> * getStatusArray(int status)
        {
            std::vector<int> * tmpData = NULL;
            switch (status)
            {
                case E_SOLVE_SEQUENCE:
                    tmpData = &m_arraySolveSequence;
                    break;
                case E_CUR_STATUS:
                    tmpData = &m_arrayCurStatus;
                    break;
                case E_NEXT_STATUS:
                    tmpData = &m_arrayNextStatus;
                    break;
            }
            return tmpData;
        }
    
    
    std::vector<int> m_arraySolveSequence;
    std::vector<int> m_arrayCurStatus;
    std::vector<int> m_arrayNextStatus;
};

static bool solverCompare(const std::vector<int> & vec1, const std::vector<int> & vec2)
{
    return (vec1.size() < vec2.size());
}

class solver
{
public:

    void process()
    {
        Cube::getInstance()->printValue();
        int originArray[] = {1,2,3,4,5,6,7,8};
        SolverData data;
        data.setStatusValue(SolverData::E_CUR_STATUS, originArray, ARRAY_SIZE(originArray));
        data.calcNextStatus(1);
        data.printAllStatus();
        stepRecursive(data);
        std::sort(m_arrayOfSolveSeq.begin(), m_arrayOfSolveSeq.end(), solverCompare);
        printSolveSeq();
        
    }

    void printSolveSeq()
        {
            fprintf(stderr, "solveSeq============\n");
            for (int i = 0; i < m_arrayOfSolveSeq.size(); i++)
            {
                fprintf(stderr, "<<<<[%04d]>>>>\n", i);
                
                std::vector<int> * tmpData = &m_arrayOfSolveSeq[i];
                
                if (tmpData)
                {
                    for (int i = 0; i < tmpData->size(); i++)
                    {
                        fprintf(stderr, "[%d]", tmpData->at(i));
                    }
                    fprintf(stderr, "\n");
                }
            }
            if (m_arrayOfSolveSeq.size() > 0)
            {
                std::vector<int> & tmpSolve = m_arrayOfSolveSeq[0];
                
                int originArray[] = {1,2,3,4,5,6,7,8};
                SolverData data;
                data.setStatusValue(SolverData::E_CUR_STATUS, originArray, ARRAY_SIZE(originArray));
                std::vector<int>::iterator it = tmpSolve.begin();
                for (; it != tmpSolve.end(); it++)
                {
                    data.calcNextStatus(*it);
                    data.printAllStatus();
                    data.copyStatus(SolverData::E_NEXT_STATUS, SolverData::E_CUR_STATUS);
                }
                
            }
        }
    
    
private:
    void stepRecursive(SolverData & prevData)
    {
        if (prevData.isSolve())
        {
            fprintf(stderr, "solve---------------------\n");
            prevData.printArray(SolverData::E_SOLVE_SEQUENCE);
            m_arrayOfSolveSeq.push_back(prevData.m_arraySolveSequence);
            
            fprintf(stderr, "solve^^^^^^^^^^^^^^^^^^^^^\n");
            return;
        }
                
        if (prevData.isReachLimit())
        {
            /*
            fprintf(stderr, "limit---------------------\n");
            prevData.printArray(SolverData::E_SOLVE_SEQUENCE);
            fprintf(stderr, "limit^^^^^^^^^^^^^^^^^^^^^\n");
            */
            return;
        }

        if (!prevData.isAGoodStratry())
        {
            /*
            fprintf(stderr, "badStratry---------------------\n");
            prevData.printArray(SolverData::E_SOLVE_SEQUENCE);
            prevData.printArray(SolverData::E_CUR_STATUS);
            prevData.printArray(SolverData::E_NEXT_STATUS);
            fprintf(stderr, "badStrary^^^^^^^^^^^^^^^^^^^^^\n");
            */
            return;
        }

        for (int i = 1; i <= Cube::MAX_CELL_NUM; i++)
        {
            SolverData curData(prevData);
            curData.copyStatus(SolverData::E_NEXT_STATUS, SolverData::E_CUR_STATUS);
            curData.calcNextStatus(i);
            //curData.printAllStatus();
            stepRecursive(curData);
        }

        
    }

    std::vector< std::vector<int> > m_arrayOfSolveSeq;

}; 

int main(int argc, char ** argv)
{
    class solver solve;
    solve.process();
    return 0;
}




