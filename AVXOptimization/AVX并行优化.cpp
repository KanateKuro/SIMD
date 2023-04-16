#include<iostream>
#include<Windows.h>
#include <immintrin.h>

using namespace std;

class TimerCounter {
public:
    TimerCounter(void);//���캯��
    ~TimerCounter(void);//��������
private:
    LARGE_INTEGER startCount;//��¼��ʼʱ��

    LARGE_INTEGER endCount;//��¼����ʱ��

    LARGE_INTEGER freq;//����CPUʱ��Ƶ��
public:
    double dbTime;//�������е�ʱ�䱣��������

public:
    void Start();//�������ʼ�㴦��ʼ��ʱ
    void Stop();//�����������㴦������ʱ
};
TimerCounter::TimerCounter(void) {
    QueryPerformanceFrequency(&freq);//��ȡ����CPUʱ��Ƶ��
}
TimerCounter::~TimerCounter(void) {
}
void TimerCounter::Start() {
    QueryPerformanceCounter(&startCount);//��ʼ��ʱ
}
void TimerCounter::Stop() {
    QueryPerformanceCounter(&endCount);//ֹͣ��ʱ
    dbTime = ((double)endCount.QuadPart - (double)startCount.QuadPart) / (double)freq.QuadPart;//��ȡʱ���
}

#define N 4096
float A[N][N], B[N][N], multi[N][N];
void AVXOptimization(float* A, float* B, float* C, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            __m256 c = _mm256_setzero_ps();
            for (int k = 0; k < n; k += 8) {
                __m256 a = _mm256_loadu_ps(&A[i * n + k]);
                __m256 b = _mm256_loadu_ps(&B[j * n + k]);
                c = _mm256_add_ps(c, _mm256_mul_ps(a, b));
            }
            __m128 c_low = _mm256_extractf128_ps(c, 0);
            __m128 c_high = _mm256_extractf128_ps(c, 1);
            c_low = _mm_add_ps(c_low, c_high);
            c_low = _mm_hadd_ps(c_low, c_low);
            c_low = _mm_hadd_ps(c_low, c_low);
            float temp;
            _mm_store_ss(&temp, c_low);
            C[i * n + j] = temp;
        }
    }
}

int main() {
    TimerCounter tc;
    for (long long i = 0; i < N; i++) {
        for (long long j = 0; j < N; j++) {
            A[i][j] = i + j;
            B[i][j] = i + j;
            multi[i][j] = 0;
        }
    }
    tc.Start();
    AVXOptimization(&A[0][0], &B[0][0], &multi[0][0], N);
    tc.Stop();
    cout << tc.dbTime;
}