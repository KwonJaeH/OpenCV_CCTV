# OpenCV_CCTV
실시간 CCTV 영상 기반, 범위 내 유클리디안 거리를 이용한 이동체 추적  :oncoming_automobile:   

- 과정과 결과 영상  
- 개선해야할 점  

----------------------------------------------------
![기본 1](https://user-images.githubusercontent.com/81853056/154875333-b395dba1-91e5-41d8-81a3-3fcd1a606bb5.png)   
해당 영상을 기준으로 이동체를 추적.  
떄마침 가로등이 경계를 주기 좋게 놓여져있어(?) 화면에 보이는 왼쪽 아래에서 두개의 가로등을 범위로 설정.    
  
## 과정과 결과 영상 :vhs:     

>  #### [1] 현재 프레임과 이전 프레임의 gray 변환  
>  ![2](https://user-images.githubusercontent.com/81853056/154875496-edf374e7-0052-494c-b70b-1bdb2978a2c2.png)
>
>  
>  #### [2] 두 프레임 간의 발생하는 차이를 이동체로 간주   
>  ![3](https://user-images.githubusercontent.com/81853056/154875802-3a5bf5f3-ce27-45c3-af39-706bab9b5092.png)   
>  이 과정에서 이동체를 간주하기 위해 배경이 단조로운 곳을 사용하는 게 좋습니다.  
>  
>  #### [3] 프레임을 이진 프레임으로 변환  
>  ![4](https://user-images.githubusercontent.com/81853056/154875849-a9b3afd8-ffa9-4f4c-ab41-31830eed6547.png)  
>  컴퓨터가 이해하기 쉽게(?) 이진 파일로 변환  
>  
>  #### [4] 이동체를 추적할 영역(범위) 설정  
>  ![image](https://user-images.githubusercontent.com/81853056/154876284-d1c63d4c-ca1e-4cf2-b61b-9e2237dd959d.png)  
>  화면과 같은 영역을 사용하고 싶어 단순하게 좌표를 이용해 영역을 설정해줬습니다.      
>  ![5](https://user-images.githubusercontent.com/81853056/154876392-6df3feef-a040-4b3f-8b0f-265ca75fa703.png)  
>  #### [5] 팽창->침식(closing) 연산을 사용하여 객체 경계 만들기  
>  ![6](https://user-images.githubusercontent.com/81853056/154876439-98d8a1cb-ca40-4c6e-a1e3-5fa16870861d.png)  
>  픽셀을 팽창해 복잡한 경계를 단순하게하고 다시 침식을 해 원래 크기로 돌려놓았습니다.  
>  #### [6] contour 찾기
>    ![7](https://user-images.githubusercontent.com/81853056/154876614-39a6c2de-228c-485f-b187-4f591785454d.png)  
>    findContours 함수를 사용해 윤곽을 추출하고 그 윤곽 대신 사각형으로 씌여줬습니다.  
>    현재 화면에서는 사각형에 랜덤한 색깔을 부여해 각 프레임마다 계속 색깔이 바뀝니다.
>  
><img src = "https://user-images.githubusercontent.com/81853056/155462202-2f47832c-1d1a-471c-bbfa-3b361564c4d0.gif" width="60%" height="60%"></img> 
>  
> #### [7] 유클리디안 거리를 이용한 같은 이동체 인식        
>![Inkedsample1_LI](https://user-images.githubusercontent.com/81853056/154877293-d68d2f28-3fa6-4db7-9e83-d0b053f130f5.jpg)
![Inkedsample2_LI](https://user-images.githubusercontent.com/81853056/154877302-bbe88767-4bb6-43d9-911f-86dfcc0aa1fd.jpg)   
> 이전 프레임의 사각형의 중심점과 다음 프레임의 사각형의 중심점의 사이의 거리를 이용하여 가까운 것을 같은 이동체라고 인식해줬습니다.   
>   
> ### 결과 영상  
> ![녹화_2022_02_20_15_36_13_664](https://user-images.githubusercontent.com/81853056/154877635-4592ee56-c57c-44a5-bf4c-eae3b4a0f59a.gif)  
    
## 개선해야할 점  
> 
> - 차가 많은 경우, 여러 차와 함께 덩어리로 인식된다.   
> - 그림자가 지는 경우, 그림자와 같이 하나의 이동체로 간주된다.  
> - 바람에 의해 CCTV가 흔들리는 경우, 프레임이 흔들리니 이동체 인식이 힘들 것으로 생각된다.    



     
