#include <iostream>
#include<ctime>
#include <windows.h>
using namespace std;

class Disk {
	int data[256]; //데이터
public:
	clock_t time;

	Disk() { //생성자
		for (int i = 0; i < 256; i++) {
			data[i] = i;
		}
	}

	int getData(int index) { 
		return data[index];
	}

	int search(int key) { //데이터가 들어있는 인덱스를 준다.
		clock_t start = clock();
		clock_t delay = 5 * CLOCKS_PER_SEC;
		while (clock() - start < delay) {}
		clock_t end = clock();
		time = end - start;

		for (int i = 0; i < 256; i++) {
			if (data[i] == key) {
				return i;
			}
		}
		return -1;
	}

	void print() {
		cout << "  Disk           " << "접근시간:" << time << endl;
		cout << "[" << data[0] << "] ";
		for (int i = 1; i < 256; i++) {
			cout << "[" << data[i] << "] ";
			if (i % 15 == 0) { cout << endl; }
		}
		cout << endl;
	}
};

class Registers {
	int data;
public:
	void set(int key) {
		data = key;
	}

	void print() {
		cout << "  Registers" << endl;
		cout << "[" << data << "]" << endl;
	}
};

class Cache {
	int data[3]; //데이터
	int data_none_time[3];//데이터를 찾지 않은 횟수
	int data_use_frequency[3]; //데이터 사용빈도 수

	int InfoList[3]; //먼저 들어온 데이터의 인덱스
public:
	int search_num; //데이터를 검색한 수
	int hit; //데이터를 캐시메모리에서 발견한 횟수
	clock_t time;

	Cache() {
		for (int i = 0; i < 3; i++) {
			data[i] = -1;
			data_none_time[i] = -1;
			data_use_frequency[i] = -1;
			InfoList[i] = -1;
		}
		hit = 0;
		search_num = 0;
	}

	//데이터가 들어있는 인덱스를 리턴
	int search(int key) {
		search_num++;
		clock_t start = clock();
		clock_t delay = 0.5 * CLOCKS_PER_SEC;
		while (clock() - start < delay) {}
		clock_t end = clock();
		time = end - start;


		for (int i = 0; i < 3; i++) {
			if (data[i] == key) { //데이터 발견
				hit++;
				data_none_time_P(i);
				data_use_frequency[i]++  ;
				return i;
			}
		}
		return -1; //데이터가 없다.
	}

	//데이터가 사용하지 않은 횟수를 갱신
	void data_none_time_P(int index) {
		for (int i = 0; i < 3; i++) {
			if (data[i] == -1)continue; //데이터가 없는 곳은 countinue 
			if (i == index) data_none_time[index] = 0; //데이터를 사용했으므로 0을 넣어준다.
			else data_none_time[i]++; //데이터를 사용하지 않았으므로 +1을 해준다.
		}
	}

	int getData(int index) {
		return data[index];
	}

	//캐시메모리가 비어있을경우 데이터 삽입
	bool setData(int key) {
		for (int i = 0; i < 3; i++) {
			if (data[i] == -1) {
				data[i] = key;
				data_none_time_P(i);
				data_use_frequency[i] = 1;
				
				for (int j = 0; j < 3; j++) { //InfoList에 들어온 순서 갱신
					if (InfoList[j] == -1) {
						InfoList[j] = i;
						return true; //삽입성공
					}
				}
			}
		}
		return false;//삽입실패
	}

	//캐시메모리가 비어있지 않음
	//데이터교체
	//1. 사용빈도가 가장 작은 데이터를 먼저 교체한다.
	//2. 만약 10번이상 동안 1번도 사용하지 않았을 경우 제일 먼저 교체해준다.
	//3. 동일한 조건의 데이터가 2개이상 있을경우 선입선출법에 따른다.

	void chageData(int key) {

		//2번 조건 수행
		for (int i = 0; i < 3; i++) {
			if (data_none_time[i] > 10) {
				data[i] = key;
				data_none_time_P(i);
				data_use_frequency[i] = 1;

				//InfoList에 들어온 순서 갱신
				int j;
				for (j = 0; InfoList[j] != i; j++) {} //InfoList에서 key을 넣은 곳의 인덱스 순위를 찾는다.
				for (int k = j; k < 2; k++) {
					int tmp = InfoList[i];
					InfoList[i] = InfoList[i + 1];
					InfoList[i + 1] = tmp;
				}
				return;
			}
		}
		
		//1번 조건 수행
		
		//사용빈도가 가장 작은데이터 찾기
		int low = 100000; //가장 작은 빈도수 저장
		int case1_index; //가장 작은 빈도수의 인덱스 저장
		for (int i = 0; i < 3; i++) {
			if (low > data_use_frequency[i]) {
				low = data_use_frequency[i];
				case1_index = i;
			}
		}

		//가장 작은 사용빈도 수의 데이터 중복확인
		int case1[3];
		int case1_num = 0;
		int k = 0;
		for (int i = 0; i < 3; i++) {
			if (low == data_use_frequency[i]) {
				case1_num++;
				for (; k < 3;) {
					case1[k] = i;//동일한 최소 사용빈도수인 데이터의 인덱스를 삽입
					k++;
					break;
				}
			}
		}

		//유일하다면 데이터 삽입
		if (case1_num == 1) {
			data[case1_index] = key;
			data_none_time_P(case1_index);
			data_use_frequency[case1_index] = 1;
			int j;
			for (j = 0; InfoList[j] != case1_index; j++) {} //InfoList에서 key을 넣은 곳의 인덱스 순위를 찾는다.
			for (int k = j; k < 2; k++) {
				int tmp = InfoList[case1_index];
				InfoList[case1_index] = InfoList[case1_index + 1];
				InfoList[case1_index + 1] = tmp;
			}
			return;
		}

		//3번 조건 수행
		//InfoList함수는 먼저 저장된 데이터의 인덱스번호를 0번부터 차레대로 저장한다.

		//동일한 최소 사용빈도수인 데이터중에 먼저 들어온 데이터를 찾는다.
		int case3_index;

		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < case1_num; k++) {
				if (InfoList[j] == case1[k]) {
					case3_index = InfoList[j];
					goto EXIT;
				}
			}
		}
	EXIT:
		data[case3_index] = key;
		data_none_time_P(case3_index);
		data_use_frequency[case3_index] = 1;

		int j, tmp;
		for (j = 0; InfoList[j] != case3_index; j++) {} //InfoList에서 key을 넣은 곳의 인덱스 순위를 찾는다.
		tmp = InfoList[j];
		for (int k = j; k < 2; k++) {
			InfoList[k] = InfoList[k + 1];
		}
		InfoList[2] = tmp;
		return;
	}

	void print() {
		cout << "  Cache          " << "접근시간:" << time << endl;
		for (int i = 0; i < 3; i++) {
			cout << "[" << data[i] << "] ";
		}
		cout << endl;
	}

};

class Memory {
	int data[15]; //데이터
	int data_none_time[15];//데이터를 찾지 않은 횟수
	int data_use_frequency[15]; //데이터 사용빈도 수

	int InfoList[15]; //먼저 들어온 데이터의 인덱스
public:
	clock_t time;

	Memory() {
		for (int i = 0; i < 15; i++) {
			data[i] = -1;
			data_none_time[i] = -1;
			data_use_frequency[i] = -1;
			InfoList[i] = -1;
		}
	}

	//데이터가 들어있는 인덱스를 리턴
	int search(int key) {
		//2초 소요시간
		clock_t start = clock();
		clock_t delay = 2 * CLOCKS_PER_SEC;
		while (clock() - start < delay) {}
		clock_t end = clock();
		time = end - start;

		for (int i = 0; i < 15; i++) {
			if (data[i] == key) { //데이터 발견
				data_none_time_P(i);
				data_use_frequency[i]++;
				return i;
			}
		}
		return -1; //데이터가 없다.
	}

	//데이터가 사용하지 않은 횟수를 갱신
	void data_none_time_P(int index) {
		for (int i = 0; i < 15; i++) {
			if (data[i] == -1)continue; //데이터가 없는 곳은 countinue 
			if (i == index) data_none_time[index] = 0; //데이터를 사용했으므로 0을 넣어준다.
			else data_none_time[i]++; //데이터를 사용하지 않았으므로 +1을 해준다.
		}
	}

	int getData(int index) {
		return data[index];
	}

	//메모리가 비어있을경우 데이터 삽입
	bool setData(int key) {
		for (int i = 0; i < 15; i++) {
			if (data[i] == -1) {
				data[i] = key;
				data_none_time_P(i);
				data_use_frequency[i] = 1;

				for (int j = 0; j < 15; j++) { //InfoList에 들어온 순서 갱신
					if (InfoList[j] == -1) {
						InfoList[j] = i;
						return true; //삽입성공
					}
				}
			}
		}
		return false;//삽입실패
	}

	//메모리가 비어있지 않음
	//데이터교체
	//1. 사용빈도가 가장 작은 데이터를 먼저 교체한다
	//2. 만약 30번 이상 동안 1번도 사용하지 않았을 경우 제일 먼저 삭제한다. 
	//3. 동일한 조건의 데이터가 2개이상 있을경우 선입선출법에 따른다.

	void chageData(int key) {

		//2번 조건 수행
		for (int i = 0; i < 15; i++) {
			if (data_none_time[i] > 30) {
				data[i] = key;
				data_none_time_P(i);
				data_use_frequency[i] = 1;

				//InfoList에 들어온 순서 갱신
				int j;
				for (j = 0; InfoList[j] != i; j++) {} //InfoList에서 key을 넣은 곳의 인덱스 순위를 찾는다.
				for (int k = j; k < 14; k++) {
					int tmp = InfoList[i];
					InfoList[i] = InfoList[i + 1];
					InfoList[i + 1] = tmp;
				}
				return;
			}
		}

		//1번 조건 수행

		//사용빈도가 가장 작은데이터 찾기
		int low = 100000; //가장 작은 빈도수 저장
		int case1_index; //가장 작은 빈도수의 인덱스 저장
		for (int i = 0; i < 15; i++) {
			if (low > data_use_frequency[i]) {
				low = data_use_frequency[i];
				case1_index = i;
			}
		}

		//가장 작은 사용빈도 수의 데이터 중복확인
		int case1[15];
		int case1_num = 0;
		int k = 0;
		for (int i = 0; i < 15; i++) {
			if (low == data_use_frequency[i]) {
				case1_num++;
				for (; k < 15;) {
					case1[k] = i;//동일한 최소 사용빈도수인 데이터의 인덱스를 삽입
					k++;
					break;
				}
			}
		}

		//유일하다면 데이터 삽입
		if (case1_num == 1) {
			data[case1_index] = key;
			data_none_time_P(case1_index);
			data_use_frequency[case1_index] = 1;
			int j;
			for (j = 0; InfoList[j] != case1_index; j++) {} //InfoList에서 key을 넣은 곳의 인덱스 순위를 찾는다.
			for (int k = j; k < 14; k++) {
				int tmp = InfoList[case1_index];
				InfoList[case1_index] = InfoList[case1_index + 1];
				InfoList[case1_index + 1] = tmp;
			}
			return;
		}

		//3번 조건 수행
		//InfoList함수는 먼저 저장된 데이터의 인덱스번호를 0번부터 차레대로 저장한다.

		//동일한 최소 사용빈도수인 데이터중에 먼저 들어온 데이터를 찾는다.
		int case3_index;

		for (int j = 0; j < 15; j++) {
			for (int k = 0; k < case1_num; k++) {
				if (InfoList[j] == case1[k]) {
					case3_index = InfoList[j];
					goto EXIT;
				}
			}
		}
	EXIT:
		data[case3_index] = key;
		data_none_time_P(case3_index);
		data_use_frequency[case3_index] = 1;

		int j, tmp;
		for (j = 0; InfoList[j] != case3_index; j++) {} //InfoList에서 key을 넣은 곳의 인덱스 순위를 찾는다.
		tmp = InfoList[j];
		for (int k = j; k < 14; k++) {
			InfoList[k] = InfoList[k + 1];
		}
		InfoList[2] = tmp;
		return;
	}

	void print() {
		cout << "  Memory         " << "접근시간:" << time << endl;
		for (int i = 0; i < 15; i++) {
			cout << "[" << data[i] << "] ";
		}
		cout << endl;
	}

};


void print(Registers r, Cache c, Memory m, Disk d, clock_t time) {
	cout << endl;
	cout << "--------------------------------------------------------------------------------" << endl;
	r.print();
	cout << endl;
	c.print();
	cout << endl;
	m.print();
	cout << endl;
	d.print();
	cout << endl;
	cout << "  캐시히트율      " << c.hit << "/" << c.search_num << endl;
	cout << "  데이터접근시간  " << time << endl;
	cout << "--------------------------------------------------------------------------------" << endl;
	cout << endl;
	c.time = m.time = d.time = 0;
}

int main() {
	Registers r;
	Cache c;
	Memory m;
	Disk d;

	int key;
	char end;
	while (true) {
		cout << "종료를 하고 싶으시면 q또는 0을 눌러주세요. 계속하고싶으시면 q를 뺀 아무거나 누르세요." << endl;
		cin >> end;
		if (end == 'q' || end == '0')break;

		cout << "데이터를 입력하세요.(0~255)" << endl;
		cin >> key;

		int c_index;
		int m_index;
		int d_index;
		if ((c_index = c.search(key)) != -1) { //캐시메모리에 데이터가 있는 경우
			r.set(c.getData(c_index));
			print(r, c, m, d, c.time);
			continue;
		}
		else if ((m_index = m.search(key)) != -1) { //메모리에 데이터가 있는 경우
			if (!c.setData(m.getData(m_index))) {
				c.chageData(m.getData(m_index));
			}
			r.set(m.getData(m_index));
			print(r, c, m, d, c.time + m.time);
			continue;
		}
		else { //디스크에 데이터가 있는 경우
			d_index = d.search(key);
			if (!m.setData(d.getData(d_index))) {
				m.chageData(d.getData(d_index));
			}
			if (!c.setData(d.getData(d_index))) {
				c.chageData(d.getData(d_index));
			}
			r.set(d.getData(d_index));
			print(r, c, m, d, c.time + m.time + d.time);
			continue;
		}
	}




	system("pause");
	return 0;
}