// =====================================================================================
// 
//       Filename:  SingletonHolder.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  07/10/07 09:39:14 CST
//       Revision:  none
//       Compiler:  g++
//         Editor:  vim (ts=4,sw=4,et)
//         Author:  Jaco Wu (), jacowu@tencent
//        Company:  Tencent
// 
//        History:
//               1. Date:
//                  Author:
//                  Modification:
// =====================================================================================

#ifndef  SINGLETONHOLDER_INC
#define  SINGLETONHOLDER_INC

template<class T>
class SingletonHolder
{
public:
    static T* Instance()
    {
        if(!pInstance_)
            pInstance_=new T;
        return pInstance_;
    }
private:
    SingletonHolder(){};
    SingletonHolder(const SingletonHolder&);
    SingletonHolder& operator=(const SingletonHolder&);
    ~SingletonHolder(){};
    static T* pInstance_;
};

template<class T>
T* SingletonHolder<T>::pInstance_=0;


#endif   /* ----- #ifndef SINGLETONHOLDER_INC  ----- */
/* vi:set ts=4: */ 
