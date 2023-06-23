#ifndef Sample2_H
#define Sample2_H
#include "EmbedSerialization.h"
#include "EmbedXrpcCommon.h"

// auto code gen ! DO NOT modify this file!
// 自动代码生成,请不要修改本文件!

#ifdef __cplusplus
extern "C"
{
#endif
    struct test
    {
        int a;
        int b;
        static inline void GetOffset_a(uint8_t *db, uint32_t *index)
        {
        }
        static inline void GetOffset_b(uint8_t *db, uint32_t *index)
        {
            GetOffset_a(db, index);
            *index += sizeof(int);
        }
        static void GetSize(uint8_t *db, uint32_t *index)
        {
            GetOffset_a(db, index);
            GetOffset_b(db, index);
            *index+=sizeof(int);
        }
    };
    typedef struct GetSumResult
    {
#ifdef ObjectStyle
        bool IsSuccess; // FieldNumber:1
        int32_t Value;  // FieldNumber:2
        int32_t len;
        test *array;
        test Value2; // FieldNumber:2
#else
    static void GetOffset_IsSuccess(uint8_t *db, uint32_t *index)
    {
        //IsSuccess=*((bool *)&db[index]);
        return;
    }
    static void GetOffset_Value(uint8_t *db, uint32_t *index)
    {
        GetOffset_IsSuccess(db, index);
        *index += sizeof(uint32_t); // 上一个的长度
        //Value=*((uint32_t *)&db[index]);
        return;
    }
    static void GetOffset_len(uint8_t *db, uint32_t *index)
    {
        GetOffset_Value(db, index);
        *index += sizeof(uint32_t); // 上一个的长度
        //len=*((uint32_t *)&db[index]);
        return;
    }
    static void GetOffset_array_element(uint8_t *db, uint32_t *index, int arrayIndex)
    {
        GetOffset_len(db, index);
        //int32_t len = *((int32_t *)(&db[*index]));
        *index += sizeof(uint32_t);//len的长度
        for (int32_t ai = 0; ai < arrayIndex; ai++)
        {
            if (ai == arrayIndex)
            {
                return;
            }
            test::GetSize(db, index);
        }
    }
    static void GetOffset_array_end(uint8_t *db, uint32_t *index)
    {
        GetOffset_len(db, index);
        int32_t len = *((int32_t *)(&db[*index]));

        *index += sizeof(uint32_t);//len的长度

        for (size_t i = 0; i < len; i++)
        {
            test::GetSize(db, index);
        }
    }
    static void GetOffset_Value2(uint8_t *db, uint32_t *index)
    {
        GetOffset_array_end(db, index);
    }
    static void GetSize(uint8_t *db, uint32_t *index)
    {
        GetOffset_Value2(db, index);
        test::GetSize(db,index);
    }
#endif
    } GetSumResult;

#define GetSumResult_IsSuccess_FieldNumber 1
#define GetSumResult_Value_FieldNumber 2

    typedef struct GetSum_Return
    {
        RequestResponseState State; // FieldNumber:1
        GetSumResult ReturnValue;   // FieldNumber:2
    } GetSum_Return;
#define GetSum_Return_State_FieldNumber 1
#define GetSum_Return_ReturnValue_FieldNumber 2

    typedef struct GetSum_Parameter
    {
        int32_t a; // FieldNumber:1
        int32_t b; // FieldNumber:2
    } GetSum_Parameter;
#define GetSum_Parameter_a_FieldNumber 1
#define GetSum_Parameter_b_FieldNumber 2

#ifdef __cplusplus
}
#endif

#endif
