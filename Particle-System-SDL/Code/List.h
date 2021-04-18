#ifndef __LIST_H__
#define __LIST_H__

template<class tdata>
struct ListItem
{
	tdata data;
	ListItem<tdata>* next;
	ListItem<tdata>* prev;

	inline ListItem(const tdata& pData)
	{
		data = pData;
		next = prev = 0;
	}
};

template<class tdata>
class List
{
public:

	ListItem<tdata>* start;
	ListItem<tdata>* end;

	unsigned int size;

	inline List()
	{
		start = end = 0;
		size = 0;
	}

	~List()
	{
		Clear();
	}

	unsigned int Count() const
	{
		return size;
	}

	ListItem<tdata>* Add(const tdata& item)
	{
		ListItem<tdata>* dataItem;
		dataItem = new ListItem<tdata>(item);

		if(!start)
		{
			start = end = dataItem;
		}
		else
		{
			dataItem->prev = end;
			end->next = dataItem;
			end = dataItem;
		}

		++size;
		return(dataItem);
	}

	void Del(ListItem<tdata>* item)
	{
		if(item->prev)
		{
			item->prev->next = item->next;

			if(item->next)
			{
				item->next->prev = item->prev;
			}
			else
			{
				end = item->prev;
			}
		}
		else
		{
			if(item->next)
			{
				item->next->prev = 0;
				start = item->next;
			}
			else
			{
				start = end = 0;
			}
		}

		RELEASE(item);
		--size;
	}

	void Clear()
	{
		ListItem<tdata>* pData = start;
		ListItem<tdata>* pNext;

		while(pData)
		{
			pNext = pData->next;
			RELEASE(pData);
			pData = pNext;
		}

		start = end = 0;
		size = 0;
	}

	tdata& operator [](const unsigned int index)
	{
		unsigned int  pos = 0;
		ListItem<tdata>* pItem = start;

		while (pItem && pos != index)
		{
			++pos;
			pItem = pItem->next;
		}

		return(pItem->data);
	}

	int Find(const tdata& data)
	{
		ListItem<tdata>* tmp = start;
		int index = 0;

		while(tmp)
		{
			if (tmp->data == data)
				return(index);

			++index;
			tmp = tmp->next;
		}
		return (-1);
	}

};

#endif
