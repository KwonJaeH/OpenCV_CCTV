# OpenCV_CCTV
CCTV 영상 기반, 범위 내 유클리디안 거리를 이용한 이동체 추적 및 차선 변경 감지  :oncoming_automobile:   

- 과정과 결과 영상  
- 개선해야할 점   

----------------------------------------------------

<img src = "https://user-images.githubusercontent.com/81853056/155045506-33766752-9c5c-404c-a862-902c760b1465.png" width="50%" height="50%"></img>     
해당 영상을 기준으로 이동체를 추적.   
임의로 정한 범위 내 이동체 추적 및 차산 변경 감지    
(* 모든 코드는 해당 영상에만 해당되게 작성함.)  
 
  
## 과정과 결과 영상 :vhs:     

> ### 차선 검출  
> * 이동체를 추적하기 전에 차선을 먼저 검출한다.  
> * 터널 내부 환경 때문에 차가 지나갈 때 생기는 그림자가 차선을 인식하기 힘들게 만드는 경우 발생.   
> * 그래서 50 프레임 동안 차선을 추출해 이동체 추적 전에 고정을 시키는 방법으로 진행.  
>    
> #### [1] 흰색(차선과 같은 색) 범위를 추출     
> <img src = "https://user-images.githubusercontent.com/81853056/155047493-abdc0ad5-4dec-41af-989a-ad3e23cbd98b.png" width="50%" height="50%"></img>    
> 터널 환경에 따라 흰색(255,255,255) 값이 아니기 때문에 알맞은 색깔 범위를 찾아줘야 함.  
> #### [2] color 이미지를 gray 이미지로 변환    
> <img src = "https://user-images.githubusercontent.com/81853056/155047679-74f22ea9-a43e-43fc-b560-ad4fd2fc260f.png" width="50%" height="50%"></img>   
> 효과적인 검출위해 gray로 변환  
> #### [3] 차선을 검출할 영역(범위) 설정  
> <img src = "https://user-images.githubusercontent.com/81853056/155047798-f6552280-9e14-48c3-bad2-938a226462ed.png" width="50%" height="50%"></img>       
> #### [4] Blur 처리   
> <img src = "https://user-images.githubusercontent.com/81853056/155047923-ff297104-3056-42cb-a695-8fe93b22dc45.png" width="50%" height="50%"></img>   
> 거친 부분을 단조롭게, 잡음 제거위해 blur 처리   
> #### [5] Canny Edge 검출   
> <img src = "https://user-images.githubusercontent.com/81853056/155048153-33304fa6-bbad-42a7-aa94-d0fac9cac60e.png" width="50%" height="50%"></img>  
> #### [6] 허프변환을 사용하여 직선 검출  
> <img src = "https://user-images.githubusercontent.com/81853056/155048357-ff19149c-259c-4da4-be30-f0f444c47b64.png" width="50%" height="50%"></img>  
> #### 차선 위치를 파악할 방정식 생성  
> 구한 직선의 양 끝 x, y 좌표를 사용해 방정식을 구하고,  
> 그 직선 그래프 기준, 현재 해당하는 이동체의 중심점의 위치에 따라 차선 선택 
> <img src = "https://user-images.githubusercontent.com/81853056/155048987-9bc55fed-7c02-4dc0-989a-9d0df6826ad7.png" width="50%" height="50%"></img>  
> 


> ### 이동체 추적   
> (**Road#1 프로젝트와 동일**) -> [보러가기](https://github.com/KwonJaeH/OpenCV_CCTV/blob/main/Road%231_one-way/README.md) 
>  #### [1] 현재 프레임과 이전 프레임의 gray 변환  
>  #### [2] 두 프레임 간의 발생하는 차이를 이동체로 간주
>  #### [3] 프레임을 이진 프레임으로 변환  
>  #### [4] 이동체를 추적할 영역(범위) 설정     
>  #### [5] 팽창->침식(closing) 연산을 사용하여 객체 경계 만들기  
>  #### [6] contour 찾기   
> #### [7] 유클리디안 거리를 이용한 같은 이동체 인식   
> < *이번에는 이동체마다 다른 색깔이 아닌 차선 별로 색깔을 구분해줌* >  
  
  
> ### 결과 영상  
> ( 한번씩 차선 변경하는 차량이 있길래 코드를 작성했지만 화면 녹화를 하려하니 법규를 잘 지키시는 분이 많았다...좋긴한데..)
>
> 기본적으로 왼쪽 lane은 **파란색**, 오른쪽 lane은 **초록색**,  차선을 변경하면 **빨간색**으로 표시하였다.   
> <img src = "https://user-images.githubusercontent.com/81853056/155074931-dcd698ec-a5c0-42c9-b81e-11ac0937feef.gif" width="70%" height="70%"></img>   
> 겨우 트럭 1대가 영역을 벗어나기 직전에 차선을 변경해 빨간색 박스로 차량을 표시했다.   
    
## 개선해야할 점  
>
> - 차가 많은 경우, 여러 차와 함께 덩어리로 인식된다.   
> - 차선 검출 과정에서 차가 많을 경우, 드물게 검출이 되지 않는 경우가 발생한다.
> - Road#1 프로젝트와는 다르게 카메라가 가까이 있어 그런지 처리해야될 객체 이미지가 커서 다소 실제 영상 속도와 차이가 있다(처리 속도때문에). 
> - 터널 내부 CCTV 높이가 대략 3~4m 이기 때문에 버스, 화물차 등 큰 차량은 많은 contour가 검출돼 인식에 어려움이 있다.
> - 화면에 차량 진입 시 카메라가 가까이 있어 차량 이미지가 깨져 초반 인식이 어렵다.
> - 차선 변경 count 시, 화면이 멈칫한다던지, 차선 변경 차량 인식에 조금 에러가 있다면 count를 제대로 하지 못한다.
