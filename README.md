# ProcessSynchornization - Operating System

### Comparing NoMutualExclusion, Semaphore , XCHG 


참조 사항 : 저는 f1의 이름을  yougnmin 으로 하였고, f2의 이름을 Lily라고 하였다. 

또, 맨처음 은행의 자금은 500000원으로 잡았다. 

또 마지막에 은행의 자금이 처음처럼 유지되는지 보기위하여, while(1)이아닌 10번만 각각 돌리기로 하였다.


## 1.
아래의 프로그램은 x86 프로세서에서 xchg 명령어를 이용하여, f1과 f2 threads 간의 mutual exclusion을 테스트하는 프로그램이다. 
아래 프로그램을 확장하여 f1과 f2가 우리은행에서 돈을 빌려서 사용하고 갚는 프로그램을 시뮬레이션 하시오. 
Mutual exclusion을 하는 경우와 안하는 경우의 결과를 비교하시오.



### Mutual Exclusion을 안하는 경우(NoMutualEx.c 첨부)


![image](https://user-images.githubusercontent.com/42762236/104193613-a748e580-5463-11eb-9944-65fdca8b40dc.png)

- Lily 쓰레드가 Critical Section안에 들어가서 돈을 출금 인출하고 , Critical section이 끝나지 않았음에도 불구하고,
Yougnmin 쓰레드가 Critical section 안에 들어가서 global 변수(은행 토탈 자금)을 바꿔버린다. 

![image](https://user-images.githubusercontent.com/42762236/104193973-0f97c700-5464-11eb-9bde-790b02ab4ca7.png)



- 마지막 토탈 자금이 처음시작할때의 자금과 달라졌음을 볼수 있다. 
이것은 쓰레드가 각각 크리티컬 섹션에서 글로번 변수를 어셈블리언어의 연산이 종료되기전에 바꿔버려 일관성이 사라졌기 때문이다.









### Mutual Exclusion을 하는 경우(xchg.c 첨부)

![image](https://user-images.githubusercontent.com/42762236/104193973-0f97c700-5464-11eb-9bde-790b02ab4ca7.png)


- Mutual exclusion을 한 결과, Lily가 크리티컬 섹션에 먼저 들어간 경우 yougnmin는 Lily가 크리티컬 섹션안에서 나온 경우 들어가고 있는 것을 볼 수 있다.

![image](https://user-images.githubusercontent.com/42762236/104194245-600f2480-5464-11eb-8b1d-2ad1f676fb65.png)

- 따라서 마지막 자금이 처음자금과 같은 금액으로 유지되었다. 



## 2. 

앞에서 구현한 프로그램을 xchg 명령어 대신에 Linux semaphore을 이용하여 재구현하고 테스트하시오..

### Semaphore 캡처화면 소프트웨어적 접근. (sema.c첨부)

![image](https://user-images.githubusercontent.com/42762236/104194254-630a1500-5464-11eb-8f87-8e5c67414d1d.png)

![image](https://user-images.githubusercontent.com/42762236/104194267-66050580-5464-11eb-9cc2-b6f57d9cf0e8.png)



- semaphore를 이용하였더니 xchg처럼 크리티컬 섹션안에 누가있다면 들어가지 않고 기다린다음 빠지면 들어가는 모습을 확인할 수 있었고 총 금액도 유지되었다.

### Semaphore 중요 코드 설명


![image](https://user-images.githubusercontent.com/42762236/104194411-a6fd1a00-5464-11eb-8555-c7b4f62f1707.png)
- 우선 헤더에 semaphore.h를 추가 해주었다.

![image](https://user-images.githubusercontent.com/42762236/104194427-ac5a6480-5464-11eb-9168-edd73851a430.png)
- 그리고 글로벌 변수로 sem_t 타입형인 bin_sem을 선언해 주었다.

![image](https://user-images.githubusercontent.com/42762236/104194431-ae242800-5464-11eb-91f7-dd27d932c385.png)
- 메인함수에서 쓰레드가 생성되기 전에 세마포어를 생성해주었고 ++1을 해주었다.

![image](https://user-images.githubusercontent.com/42762236/104194439-b0868200-5464-11eb-824e-103062a0ce3a.png)

- 각각의 쓰레드 함수 안에서는 크리티컬 섹션에 들어가기 전과 후에 wait과 signal을 걸어주었다.
