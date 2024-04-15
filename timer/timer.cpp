#include<timer.h>

//添加新的定时器
void sort_timer_lst::add_timer(util_timer *timer)
{
    if(!timer)return;
    if(!head)head = tail = timer;
    //如果当前定时器的超时时间小于链表头部的定时器，特殊处理
    if(timer->expire < head->expire)
    {
        timer->next = head;
        head->prev = timer;
        head = timer;
        return;
    }
    add_timer(timer, head);
}
void sort_timer_lst::add_timer(util_timer *timer, util_timer *head)
{
    util_timer *prev = head;
    util_timer *tmp = head->next;
    while(tmp)
    {
        if(tmp->expire > timer->expire)
        {
            prev->next = timer;
            timer->next = tmp;
            timer->prev = prev;
            tmp->prev = tmp;
            break;
        }
        prev = tmp;
        tmp = tmp->next;
    }
    //如果当前timer的expire大于链表上所有的定时器，特殊处理
    if(!tmp)
    {
        prev->next = timer;
        timer->prev = prev;
        timer->next = NULL;
        tail = timer;
    }
}
void sort_timer_lst::del_timer(util_timer *timer)
{
    
}