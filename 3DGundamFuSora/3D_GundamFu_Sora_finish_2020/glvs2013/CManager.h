#pragma once

#include <vector>
#include <list>

template <class T>
class CManager {
protected:
//	std::arraytor<T> array;
	std::list<T> array;

public:
	~CManager();
	void Add(T t);
	void Update();
	void Render();
	void Destroy();
};


template <class T>
void  CManager<T>::Add(T t) {
	array.push_back(t);
}

template <class T>
void CManager<T>::Update() {
//	std::arraytor<T>::iterator itr;
	std::list<T>::iterator itr;
	for (itr = array.begin(); itr != array.end(); itr++) {
		(*itr)->Update();
		if (!(*itr)->m_alive) {
			delete *itr;
			itr = array.erase(itr);
			if (itr == array.end())
				break;
		}
	}
}

template <class T>
void CManager<T>::Render() {
	//	std::vector<T>::iterator itr;
	std::list<T>::iterator itr;
	for (itr = array.begin(); itr != array.end(); itr++) {
		(*itr)->Render();
	}
}

template <class T>
void CManager<T>::Destroy() {
	//	std::arraytor<T>::iterator itr;
	std::list<T>::iterator itr;
	for (itr = array.begin(); itr != array.end(); itr++) {
		delete *itr;
		itr = array.erase(itr);
		if (itr == array.end())
			break;
	}
}

template <class T>
CManager<T>::~CManager() {
	Destroy();
}
