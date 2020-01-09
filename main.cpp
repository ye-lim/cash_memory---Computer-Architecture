#include <iostream>
#include<ctime>
#include <windows.h>
using namespace std;

class Disk {
	int data[256]; //������
public:
	clock_t time;

	Disk() { //������
		for (int i = 0; i < 256; i++) {
			data[i] = i;
		}
	}

	int getData(int index) { 
		return data[index];
	}

	int search(int key) { //�����Ͱ� ����ִ� �ε����� �ش�.
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
		cout << "  Disk           " << "���ٽð�:" << time << endl;
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
	int data[3]; //������
	int data_none_time[3];//�����͸� ã�� ���� Ƚ��
	int data_use_frequency[3]; //������ ���� ��

	int InfoList[3]; //���� ���� �������� �ε���
public:
	int search_num; //�����͸� �˻��� ��
	int hit; //�����͸� ĳ�ø޸𸮿��� �߰��� Ƚ��
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

	//�����Ͱ� ����ִ� �ε����� ����
	int search(int key) {
		search_num++;
		clock_t start = clock();
		clock_t delay = 0.5 * CLOCKS_PER_SEC;
		while (clock() - start < delay) {}
		clock_t end = clock();
		time = end - start;


		for (int i = 0; i < 3; i++) {
			if (data[i] == key) { //������ �߰�
				hit++;
				data_none_time_P(i);
				data_use_frequency[i]++  ;
				return i;
			}
		}
		return -1; //�����Ͱ� ����.
	}

	//�����Ͱ� ������� ���� Ƚ���� ����
	void data_none_time_P(int index) {
		for (int i = 0; i < 3; i++) {
			if (data[i] == -1)continue; //�����Ͱ� ���� ���� countinue 
			if (i == index) data_none_time[index] = 0; //�����͸� ��������Ƿ� 0�� �־��ش�.
			else data_none_time[i]++; //�����͸� ������� �ʾ����Ƿ� +1�� ���ش�.
		}
	}

	int getData(int index) {
		return data[index];
	}

	//ĳ�ø޸𸮰� ���������� ������ ����
	bool setData(int key) {
		for (int i = 0; i < 3; i++) {
			if (data[i] == -1) {
				data[i] = key;
				data_none_time_P(i);
				data_use_frequency[i] = 1;
				
				for (int j = 0; j < 3; j++) { //InfoList�� ���� ���� ����
					if (InfoList[j] == -1) {
						InfoList[j] = i;
						return true; //���Լ���
					}
				}
			}
		}
		return false;//���Խ���
	}

	//ĳ�ø޸𸮰� ������� ����
	//�����ͱ�ü
	//1. ���󵵰� ���� ���� �����͸� ���� ��ü�Ѵ�.
	//2. ���� 10���̻� ���� 1���� ������� �ʾ��� ��� ���� ���� ��ü���ش�.
	//3. ������ ������ �����Ͱ� 2���̻� ������� ���Լ������ ������.

	void chageData(int key) {

		//2�� ���� ����
		for (int i = 0; i < 3; i++) {
			if (data_none_time[i] > 10) {
				data[i] = key;
				data_none_time_P(i);
				data_use_frequency[i] = 1;

				//InfoList�� ���� ���� ����
				int j;
				for (j = 0; InfoList[j] != i; j++) {} //InfoList���� key�� ���� ���� �ε��� ������ ã�´�.
				for (int k = j; k < 2; k++) {
					int tmp = InfoList[i];
					InfoList[i] = InfoList[i + 1];
					InfoList[i + 1] = tmp;
				}
				return;
			}
		}
		
		//1�� ���� ����
		
		//���󵵰� ���� ���������� ã��
		int low = 100000; //���� ���� �󵵼� ����
		int case1_index; //���� ���� �󵵼��� �ε��� ����
		for (int i = 0; i < 3; i++) {
			if (low > data_use_frequency[i]) {
				low = data_use_frequency[i];
				case1_index = i;
			}
		}

		//���� ���� ���� ���� ������ �ߺ�Ȯ��
		int case1[3];
		int case1_num = 0;
		int k = 0;
		for (int i = 0; i < 3; i++) {
			if (low == data_use_frequency[i]) {
				case1_num++;
				for (; k < 3;) {
					case1[k] = i;//������ �ּ� ���󵵼��� �������� �ε����� ����
					k++;
					break;
				}
			}
		}

		//�����ϴٸ� ������ ����
		if (case1_num == 1) {
			data[case1_index] = key;
			data_none_time_P(case1_index);
			data_use_frequency[case1_index] = 1;
			int j;
			for (j = 0; InfoList[j] != case1_index; j++) {} //InfoList���� key�� ���� ���� �ε��� ������ ã�´�.
			for (int k = j; k < 2; k++) {
				int tmp = InfoList[case1_index];
				InfoList[case1_index] = InfoList[case1_index + 1];
				InfoList[case1_index + 1] = tmp;
			}
			return;
		}

		//3�� ���� ����
		//InfoList�Լ��� ���� ����� �������� �ε�����ȣ�� 0������ ������� �����Ѵ�.

		//������ �ּ� ���󵵼��� �������߿� ���� ���� �����͸� ã�´�.
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
		for (j = 0; InfoList[j] != case3_index; j++) {} //InfoList���� key�� ���� ���� �ε��� ������ ã�´�.
		tmp = InfoList[j];
		for (int k = j; k < 2; k++) {
			InfoList[k] = InfoList[k + 1];
		}
		InfoList[2] = tmp;
		return;
	}

	void print() {
		cout << "  Cache          " << "���ٽð�:" << time << endl;
		for (int i = 0; i < 3; i++) {
			cout << "[" << data[i] << "] ";
		}
		cout << endl;
	}

};

class Memory {
	int data[15]; //������
	int data_none_time[15];//�����͸� ã�� ���� Ƚ��
	int data_use_frequency[15]; //������ ���� ��

	int InfoList[15]; //���� ���� �������� �ε���
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

	//�����Ͱ� ����ִ� �ε����� ����
	int search(int key) {
		//2�� �ҿ�ð�
		clock_t start = clock();
		clock_t delay = 2 * CLOCKS_PER_SEC;
		while (clock() - start < delay) {}
		clock_t end = clock();
		time = end - start;

		for (int i = 0; i < 15; i++) {
			if (data[i] == key) { //������ �߰�
				data_none_time_P(i);
				data_use_frequency[i]++;
				return i;
			}
		}
		return -1; //�����Ͱ� ����.
	}

	//�����Ͱ� ������� ���� Ƚ���� ����
	void data_none_time_P(int index) {
		for (int i = 0; i < 15; i++) {
			if (data[i] == -1)continue; //�����Ͱ� ���� ���� countinue 
			if (i == index) data_none_time[index] = 0; //�����͸� ��������Ƿ� 0�� �־��ش�.
			else data_none_time[i]++; //�����͸� ������� �ʾ����Ƿ� +1�� ���ش�.
		}
	}

	int getData(int index) {
		return data[index];
	}

	//�޸𸮰� ���������� ������ ����
	bool setData(int key) {
		for (int i = 0; i < 15; i++) {
			if (data[i] == -1) {
				data[i] = key;
				data_none_time_P(i);
				data_use_frequency[i] = 1;

				for (int j = 0; j < 15; j++) { //InfoList�� ���� ���� ����
					if (InfoList[j] == -1) {
						InfoList[j] = i;
						return true; //���Լ���
					}
				}
			}
		}
		return false;//���Խ���
	}

	//�޸𸮰� ������� ����
	//�����ͱ�ü
	//1. ���󵵰� ���� ���� �����͸� ���� ��ü�Ѵ�
	//2. ���� 30�� �̻� ���� 1���� ������� �ʾ��� ��� ���� ���� �����Ѵ�. 
	//3. ������ ������ �����Ͱ� 2���̻� ������� ���Լ������ ������.

	void chageData(int key) {

		//2�� ���� ����
		for (int i = 0; i < 15; i++) {
			if (data_none_time[i] > 30) {
				data[i] = key;
				data_none_time_P(i);
				data_use_frequency[i] = 1;

				//InfoList�� ���� ���� ����
				int j;
				for (j = 0; InfoList[j] != i; j++) {} //InfoList���� key�� ���� ���� �ε��� ������ ã�´�.
				for (int k = j; k < 14; k++) {
					int tmp = InfoList[i];
					InfoList[i] = InfoList[i + 1];
					InfoList[i + 1] = tmp;
				}
				return;
			}
		}

		//1�� ���� ����

		//���󵵰� ���� ���������� ã��
		int low = 100000; //���� ���� �󵵼� ����
		int case1_index; //���� ���� �󵵼��� �ε��� ����
		for (int i = 0; i < 15; i++) {
			if (low > data_use_frequency[i]) {
				low = data_use_frequency[i];
				case1_index = i;
			}
		}

		//���� ���� ���� ���� ������ �ߺ�Ȯ��
		int case1[15];
		int case1_num = 0;
		int k = 0;
		for (int i = 0; i < 15; i++) {
			if (low == data_use_frequency[i]) {
				case1_num++;
				for (; k < 15;) {
					case1[k] = i;//������ �ּ� ���󵵼��� �������� �ε����� ����
					k++;
					break;
				}
			}
		}

		//�����ϴٸ� ������ ����
		if (case1_num == 1) {
			data[case1_index] = key;
			data_none_time_P(case1_index);
			data_use_frequency[case1_index] = 1;
			int j;
			for (j = 0; InfoList[j] != case1_index; j++) {} //InfoList���� key�� ���� ���� �ε��� ������ ã�´�.
			for (int k = j; k < 14; k++) {
				int tmp = InfoList[case1_index];
				InfoList[case1_index] = InfoList[case1_index + 1];
				InfoList[case1_index + 1] = tmp;
			}
			return;
		}

		//3�� ���� ����
		//InfoList�Լ��� ���� ����� �������� �ε�����ȣ�� 0������ ������� �����Ѵ�.

		//������ �ּ� ���󵵼��� �������߿� ���� ���� �����͸� ã�´�.
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
		for (j = 0; InfoList[j] != case3_index; j++) {} //InfoList���� key�� ���� ���� �ε��� ������ ã�´�.
		tmp = InfoList[j];
		for (int k = j; k < 14; k++) {
			InfoList[k] = InfoList[k + 1];
		}
		InfoList[2] = tmp;
		return;
	}

	void print() {
		cout << "  Memory         " << "���ٽð�:" << time << endl;
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
	cout << "  ĳ����Ʈ��      " << c.hit << "/" << c.search_num << endl;
	cout << "  ���������ٽð�  " << time << endl;
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
		cout << "���Ḧ �ϰ� �����ø� q�Ǵ� 0�� �����ּ���. ����ϰ�����ø� q�� �� �ƹ��ų� ��������." << endl;
		cin >> end;
		if (end == 'q' || end == '0')break;

		cout << "�����͸� �Է��ϼ���.(0~255)" << endl;
		cin >> key;

		int c_index;
		int m_index;
		int d_index;
		if ((c_index = c.search(key)) != -1) { //ĳ�ø޸𸮿� �����Ͱ� �ִ� ���
			r.set(c.getData(c_index));
			print(r, c, m, d, c.time);
			continue;
		}
		else if ((m_index = m.search(key)) != -1) { //�޸𸮿� �����Ͱ� �ִ� ���
			if (!c.setData(m.getData(m_index))) {
				c.chageData(m.getData(m_index));
			}
			r.set(m.getData(m_index));
			print(r, c, m, d, c.time + m.time);
			continue;
		}
		else { //��ũ�� �����Ͱ� �ִ� ���
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