# ** 수정 중 ** 2/23    
  
# OpenCV_CCTV
실시간 CCTV 영상 기반, 범위 내 차량 속도 측정  :oncoming_automobile:   

- 과정과 결과 영상  
- 개선해야할 점   

----------------------------------------------------

<img src = "https://user-images.githubusercontent.com/81853056/155316196-108783d3-6b6d-4dde-8434-758be6806df9.png" width="50%" height="50%"></img>     
해당 영상을 기준으로 속도 측정   
임의로 정한 범위 내 차량 속도 측정    
(* 모든 코드는 해당 영상에만 해당되게 작성함.)  
  
    
### **가정**
1. 차선 간격은 자동차 전용 도로 노면 표시 규제에 따라 정확하다.  
2. 도로는 직선도로이다.  
3. 속도를 측정하는 범위 내에서는 차량이 차선을 변경하지 않는다.  
4. (차로 별로 속도를 구할 경우) 차선 검출이 수월한 화창한 낮에 진행한다.  


  
## 과정과 결과 영상 :vhs:     

> ## 속도 측정 방법  
> <img src = "https://user-images.githubusercontent.com/81853056/155317792-ab5f2a1c-5a11-4df7-9886-4882976e2a23.png" width="50%" height="50%"></img>     
> source - 교통노면표시 설치 관리 메뉴얼(도로표지병편 20.3.26. 개정)  
> 점선의 도색 길이, 빈 길이를 사용해 거리를 유추해 속도를 측정  
> ![image](https://user-images.githubusercontent.com/81853056/155318937-709bcc42-2d89-4d22-9688-1a5edc924800.png)  
> 범위 안의 점선 시작 부분과 점선 끝 부분을 속도 측정 범위로 설정 ( 도색 - 3 , 빈 - 2  =  50m)  
>  
> [범위와 설정 이유 이미지]   
  
> ### 차선 검출  
> * 이동체를 추적하기 전에 차선을 먼저 검출한다.  
>    
> #### [1] 흰색(차선과 같은 색) 범위를 추출     
> <img src = "https://user-images.githubusercontent.com/81853056/155317792-ab5f2a1c-5a11-4df7-9886-4882976e2a23.png" width="50%" height="50%"></img>    
> #### [2] color 이미지를 gray 이미지로 변환    
> <img src = "https://user-images.githubusercontent.com/81853056/155047679-74f22ea9-a43e-43fc-b560-ad4fd2fc260f.png" width="50%" height="50%"></img>   
> 효과적인 검출위해 gray로 변환  
> #### [3] 차선을 검출할 영역(범위) 설정  
> <img src = "https://user-images.githubusercontent.com/81853056/155047798-f6552280-9e14-48c3-bad2-938a226462ed.png" width="50%" height="50%"></img>       
> #### [4] Blur 처리   
> <img src = "https://user-images.githubusercontent.com/81853056/155047923-ff297104-3056-42cb-a695-8fe93b22dc45.png" width="50%" height="50%"></img>     
> #### [5] Canny Edge 검출   
> <img src = "https://user-images.githubusercontent.com/81853056/155048153-33304fa6-bbad-42a7-aa94-d0fac9cac60e.png" width="50%" height="50%"></img>  
> #### [6] 허프변환을 사용하여 직선 검출  
> <img src = "https://user-images.githubusercontent.com/81853056/155048357-ff19149c-259c-4da4-be30-f0f444c47b64.png" width="50%" height="50%"></img>  
> #### 차선 위치를 파악할 방정식 생성  
> 구한 직선의 양 끝 x, y 좌표를 사용해 방정식을 구하고,  
> 그 직선 그래프 기준, 현재 해당하는 이동체의 중심점의 위치에 따라 차선 선택 
> <img src = "https://user-images.githubusercontent.com/81853056/155048987-9bc55fed-7c02-4dc0-989a-9d0df6826ad7.png" width="70%" height="70%"></img>  
> 


> ### 객체 검출   
> (**Road#1 프로젝트와 동일**) -> [보러가기](https://github.com/KwonJaeH/OpenCV_CCTV/blob/main/Road%231_one-way/README.md) 
>  #### [1] 현재 프레임과 이전 프레임의 gray 변환  
>  #### [2] 두 프레임 간의 발생하는 차이를 이동체로 간주
>  #### [3] 프레임을 이진 프레임으로 변환  
>  #### [4] 이동체를 추적할 영역(범위) 설정     
>  #### [5] 팽창->침식(closing) 연산을 사용하여 객체 경계 만들기  
>  #### [6] contour 찾기   
> #### [7] 유클리디안 거리를 이용한 같은 이동체 인식 및 start line 진입 시간 저장   
> < *Road#1과 Road#2와 다르게 설정 영역 진입, 그리고 벗어날 때만 색깔 박스로 표시함* >  
  
  
> ### 결과 영상  
> (readme gif 용량 제한이 있어 영상이 짧습니다.)   
>   
> 범위에 진입할 때 **초록색** 깜빡, 범위를 벗어날 때 **빨간색** 깜빡으로 표시하였다.  
>   
> #1 Case - 차선 구분 없이 측정  
> <img src = "https://user-images.githubusercontent.com/81853056/155324347-190da71b-35ba-4ea3-9f1f-01a02bd4b999.gif" width="50%" height="50%"></img>     
>  
    
## 개선해야할 점  
>
> - 차가 많은 경우, 여러 차와 함께 덩어리로 인식된다.   
> - 터널 내부 CCTV 높이가 대략 3~4m 이기 때문에 버스, 화물차 등 높이가 높은 차량은 카메라가 가까이 있어 많은 contour가 검출돼 인식에 어려움이 있다.
> - 
> - 
>  
