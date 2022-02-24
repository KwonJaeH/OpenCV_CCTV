
# OpenCV_CCTV
실시간 CCTV 영상 기반, 범위 내 차량 속도 측정  :oncoming_automobile:   

- 과정과 결과 영상  
- 개선해야할 점   

----------------------------------------------------

<img src = "https://user-images.githubusercontent.com/81853056/155456071-0668ecbc-61c0-40d9-9714-a5d7d263c25c.png" width="50%" height="50%"></img>     
해당 영상을 기준으로 속도 측정   
임의로 정한 범위 내 차량 속도 측정    
(* 모든 코드는 해당 영상에만 해당되게 작성함.)  
  
    
### **가정**
1. 차선 간격은 자동차 전용 도로 노면 표시 규제에 따라 정확하다.  
2. 도로는 직선도로이다.  
3. 속도를 측정하는 범위 내에서는 차량이 차선을 변경하지 않는다.  
4. 속도 측정을 할 때는 교통량이  않다.  
(차로(Lane) 별로 속도를 구할 경우)   
5. 차선 검출이 수월한 화창한 낮에 진행한다.   


<br></br>
## 과정과 결과 영상 :vhs:     

> ## 속도 측정 방법  
> <img src = "https://user-images.githubusercontent.com/81853056/155317792-ab5f2a1c-5a11-4df7-9886-4882976e2a23.png" width="50%" height="50%"></img>     
> source - 교통노면표시 설치 관리 메뉴얼(도로표지병편 20.3.26. 개정)  
> 점선의 도색 길이, 빈 길이를 사용해 거리를 유추해 속도를 측정  
> <img src = "https://user-images.githubusercontent.com/81853056/155456167-80304b1f-5af5-4c1f-bc8e-5808c5126345.png" width="50%" height="50%"></img>   
> 범위 안의 점선 시작 부분과 점선 끝 부분을 속도 측정 범위로 설정 ( ex. 도색 - 4 , 빈 - 3  =  70m)  
>  ( * 화면 마다 기준이 다름. 본인이 설정 )    
>    
>    
> <img src = "https://user-images.githubusercontent.com/81853056/155456469-ed0485b4-3963-4001-81e9-18aafa2eff20.png" width="70%" height="70%"></img>   
> 화면에 보이는 얇은 **초록색, 빨간선**이 각각 start line , end line이다.   
> 저렇게 각도를 잡은 이유는 화면에 보이는 차량의 전방 하단 부분(**파란선**)과 평행하게 기준을 잡았다.    
>      
> #### [1] 차량 마다 Start Line 지날 때의 시간(Start time)을 저장     
> #### [2] 차량이 End Line 지날 때, 저장된 Start time 과 End time시간의 차이로 총 이동시간(Drive time) 계산   
> #### [3] '거리 = 속력 x 시간' 공식 사용하여 값 도출   
  
<br></br>
> ## 차선 검출  
> (**Road#2 프로젝트와 동일**) ->  [보러가기](https://github.com/KwonJaeH/OpenCV_CCTV/blob/main/Road%232_one-way%5BTunnel(%EC%B0%A8%EC%84%A0%20%EB%B3%80%EA%B2%BD)%5D/README.md)   
>    
> #### [1] 흰색(차선과 같은 색) 범위를 추출       
> #### [2] color 이미지를 gray 이미지로 변환
> #### [3] 차선을 검출할 영역(범위) 설정         
> #### [4] Blur 처리       
> #### [5] Canny Edge 검출     
> #### [6] 허프변환을 사용하여 직선 검출   
> #### 차선 위치를 파악할 방정식 생성  
> <img src = "https://user-images.githubusercontent.com/81853056/155456914-47812be2-d541-4d2b-9211-a1082ef5f034.png" width="50%" height="50%"></img>  
> **노란선** - 차선(점선)  
  
<br></br>
> ## 객체 검출   
> (**Road#1 프로젝트와 동일**) -> [보러가기](https://github.com/KwonJaeH/OpenCV_CCTV/blob/main/Road%231_one-way/README.md) 
>  #### [1] 현재 프레임과 이전 프레임의 gray 변환  
>  #### [2] 두 프레임 간의 발생하는 차이를 이동체로 간주
>  #### [3] 프레임을 이진 프레임으로 변환  
>  #### [4] 이동체를 추적할 영역(범위) 설정     
>  #### [5] 팽창->침식(closing) 연산을 사용하여 객체 경계 만들기  
>  #### [6] contour 찾기   
> #### [7] 유클리디안 거리를 이용한 같은 이동체 인식 및 start line 진입 시간 저장   
> < *Road#1과 Road#2와 다르게 설정 영역 진입, 그리고 벗어날 때만 색깔 박스로 표시함* >  
  
<br></br>
> ## 결과 영상  
> (readme gif 용량 제한이 있어 영상이 짧습니다.)   
>   
> 범위에 진입할 때 **초록색** 깜빡, 범위를 벗어날 때 **빨간색** 깜빡으로 표시하였다.  
>   
> **#1 Case - 차선 구분 x, 속도 측정**   
> <img src = "https://user-images.githubusercontent.com/81853056/155324347-190da71b-35ba-4ea3-9f1f-01a02bd4b999.gif" width="70%" height="70%"></img>   
> 
> **#2 Case - 차선 구분 o, 속도 측정**     
>  <img src = "https://user-images.githubusercontent.com/81853056/155460427-3c9624b9-661d-435e-8115-54330316026a.gif" width="70%" height="70%"></img>       
>    
>  <img src = "https://user-images.githubusercontent.com/81853056/155460433-81f0b893-2d55-47fc-955c-d06209ae0e43.gif" width="70%" height="70%"></img>    

<br></br>  
## 개선해야할 점  
>
> - 차가 많은 경우, 여러 차와 함께 덩어리로 인식된다.   
> - 그림자가 질 경우, 같은 덩어리로 인식된다.  
> - 차가 조금만 여러대일 경우 & 이동체 인식이 잘 안되는 경우, 터무니 없는 속도를 출력한다.   
> - 결론은 이동체 인식을 더 명확하게 할 줄 알아야한다.
>  
