#include <gtest/gtest.h>
#include <iostream>

using namespace std;

template<typename T>
class CMyList
{
private:
    struct MyListItem
    {
        T value{};
        MyListItem *nextItem = nullptr;
        MyListItem *prevItem = nullptr;
    };

public:
    class ListIterator
    {
        friend class CMyList<T>;

    public:
        ListIterator() = default;

        ListIterator(MyListItem *listItem)
        {
            item = listItem;
        }

        ListIterator &operator++()
        {
            if (item == nullptr || item->nextItem == nullptr)
            {
                throw out_of_range("can't iterate more");
            }
            item = item->nextItem;
            return *this;
        }

        ListIterator &operator--()
        {
            if (item == nullptr || item->prevItem == nullptr)
            {
                throw out_of_range("can't iterate more");
            }
            item = item->prevItem;
            return *this;
        }

        T operator*() const
        { return item->value; }

    private:
        MyListItem *item = nullptr;
    };

    class ReverseListIterator
    {
    public:
        ReverseListIterator(ListIterator iterator)
        {
            listIterator = iterator;
        }

        ListIterator &operator++()
        {
            return --listIterator;
        }

        ListIterator &operator--()
        {
            return ++listIterator;
        }

        T operator*() const
        { return *listIterator; }

    private:
        ListIterator listIterator;
    };

    CMyList()
    {
        lastItem = new MyListItem{};
    }

    CMyList(CMyList<T> const &cMyList)
    {
        if (cMyList.len == 0)
        {
            lastItem = new MyListItem{};
            return;
        }
        MyListItem *itemToCopy = cMyList.firstItem;
        MyListItem *prevItem = nullptr;
        do
        {
            auto *newItem = new MyListItem{.value = itemToCopy->value, .prevItem = prevItem};
            if (prevItem != nullptr)
            {
                prevItem->nextItem = newItem;
            }
            prevItem = newItem;
            itemToCopy = itemToCopy->nextItem;
        } while (itemToCopy != nullptr);
    }

    ~CMyList()
    {
        if (len == 0)
        {
            return;
        }
        MyListItem *item = firstItem;
        do
        {
            MyListItem *next = item->nextItem;
            delete item;
            item = next;
        } while (item != nullptr);
    }

    void AddToStart(const T value)
    {
        MyListItem *newItem = new MyListItem{.value = value};
        if (firstItem == nullptr)
        {
            newItem->nextItem = lastItem;
            firstItem = newItem;
            lastItem->prevItem = newItem;
        }
        else
        {
            newItem->nextItem = firstItem;
            firstItem->prevItem = newItem;
            firstItem = newItem;
        }
        len++;
    }

    void AddToEnd(const T value)
    {
        MyListItem *newItem = new MyListItem{.value =  value, .nextItem = lastItem};
        if (firstItem == nullptr)
        {
            firstItem = newItem;
        }
        else
        {
            newItem->prevItem = lastItem->prevItem;
            lastItem->prevItem->nextItem = newItem;
        }
        lastItem->prevItem = newItem;
        len++;
    }

    void Insert(ListIterator listIterator, const T value)
    {
        MyListItem *newItem = new MyListItem{.value =  value, .nextItem = listIterator.item->nextItem, .prevItem = listIterator.item};
        listIterator.item->nextItem->prevItem = newItem;
        listIterator.item->nextItem = newItem;
        len++;
    }

    void DeleteBy(ListIterator listIterator)
    {
        listIterator.item->prevItem->nextItem = listIterator.item->nextItem;
        listIterator.item->nextItem->prevItem = listIterator.item->prevItem;
        delete listIterator.item;
        len--;
    }

    int Len()
    {
        return len;
    }

    ListIterator begin()
    {
        return *new ListIterator(firstItem);
    }

    ListIterator end()
    {
        return *new ListIterator(lastItem);
    }

    ReverseListIterator rbegin()
    {
        return *new ReverseListIterator(end());
    }

    ReverseListIterator rend()
    {
        return *new ReverseListIterator(begin());
    }

private:
    MyListItem *firstItem = nullptr;
    MyListItem *lastItem = nullptr;
    int len = 0;
};

class TestCMyList : public ::testing::Test
{
};

TEST_F(TestCMyList, shouldAddElements)
{
    CMyList<float> list = *new CMyList<float>();
    ASSERT_EQ(list.Len(), 0);
    list.AddToStart(32.393);
    ASSERT_EQ(list.Len(), 1);
    list.AddToEnd(920);
    ASSERT_EQ(list.Len(), 2);
}

TEST_F(TestCMyList, shouldCallDesctructor)
{
    CMyList<string> list = *new CMyList<string>();
    list.AddToStart("abc");
    list.AddToEnd("def");
    CMyList<string> list2 = *new CMyList<string>(list);
}

TEST_F(TestCMyList, shouldCopyItems)
{
    CMyList<string> list = *new CMyList<string>();
    list.AddToStart("abc");
    list.AddToEnd("def");
    CMyList<string> list2 = *new CMyList<string>(list);
    list2.AddToEnd("ghi");
    auto it = list.begin();
    ASSERT_EQ("abc", *it);
}

TEST_F(TestCMyList, shouldIterate)
{
    CMyList<string> list = *new CMyList<string>();
    list.AddToStart("abc");
    list.AddToEnd("def");
    list.AddToEnd("ghi");
    auto it = list.begin();
    ASSERT_EQ("abc", *it);
    ++it;
    ASSERT_EQ("def", *it);
    --it;
    ASSERT_EQ("abc", *it);
    ++it;
    ++it;
    ASSERT_EQ("ghi", *it);
    ++it;
    ASSERT_THROW(++it, out_of_range);
}

TEST_F(TestCMyList, shouldInsertByIterator)
{
    CMyList<string> list = *new CMyList<string>();
    list.AddToStart("a");
    list.AddToEnd("def");
    list.AddToEnd("ghi");
    auto it = list.begin();
    list.Insert(it, "bc");
    ASSERT_EQ(list.Len(), 4);
    it = list.begin();
    ++it;
    ASSERT_EQ("bc", *it);
}

TEST_F(TestCMyList, shouldDeleteByIterator)
{
    CMyList<string> list = *new CMyList<string>();
    list.AddToStart("abc");
    list.AddToEnd("def");
    list.AddToEnd("ghi");
    auto it = list.begin();
    ++it;
    list.DeleteBy(it);
    ASSERT_EQ(list.Len(), 2);
    it = list.begin();
    ASSERT_EQ("abc", *it);
    ++it;
    ASSERT_EQ("ghi", *it);
}

TEST_F(TestCMyList, shouldIterateReversely)
{
    CMyList<string> list = *new CMyList<string>();
    list.AddToStart("abc");
    list.AddToEnd("def");
    list.AddToEnd("ghi");
    auto it = list.rbegin();
    ++it;
    ASSERT_EQ("ghi", *it);
    ++it;
    ASSERT_EQ("def", *it);
    ++it;
    ASSERT_EQ("abc", *it);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}