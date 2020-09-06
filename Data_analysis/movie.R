##############################################################################################################################
# 데이터분석
# 조이름 : 홍송조
# 주제 : 영화의 흥행공식 요소 분석
##############################################################################################################################
#데이터 가공

#boxoffice라는 csv파일을 factor형이 생기지 않도록 읽어온다.
d = read.csv("CSV/boxoffice.csv", stringsAsFactors = F)

#company부분을 factor형으로 바꿔준다.
d$company = factor(d$company)

#season이라는 열을 하나 더 추가한다.
d$season = "spring"
#month에 따라 값을 바꿔주고 factor형으로 바꿔준다.
d[d$month %in% c(3,4,5),9] = "spring"
d[d$month %in% c(6,7,8),9] = "summer"
d[d$month %in% c(9,10,11),9] = "fall"
d[d$month %in% c(12,1,2),9] = "winter"
d$season = factor(d$season)

#num.movie라는 이름의 행렬을 만들어서 각 월의 개수를 세어서 저장한다. 그 후 data.frame형으로 바꿔준다.
num.movie = matrix(0,ncol = 12,nrow=1);
month_count=function(d){
  num.movie = matrix(0,ncol = 12,nrow=1);
  for(i in 1:400){
    for(j in 1:12){
      if(d[i,3] == j){
        num.movie[1,j]=num.movie[1,j]+1;
      }
    }
  }
  return (num.movie);
}
num.movie=month_count(d);
num.movie = data.frame(num.movie);
names(num.movie) = c(1:12);

#avg.rank라는 이름의 행렬을 만들어서 각 월의 rank의 평균을 계산하여 저장한다. 그 후 data.frame형으로 바꿔준다.
avg.rank = matrix(0,ncol = 12,nrow=1);
rank_avg = function(d,num.movie){
  avg.rank = matrix(0,ncol = 12,nrow=1);
  sum = matrix(0,ncol = 12,nrow=1);
  for(i in 1:400){
    for(j in 1:12){
      if(d[i,3] == j){
        sum[1,j] = sum[1,j] + d[i,1];
      }
    }
  }
  for(i in 1:12){
    avg.rank[1,i] = sum[1,i]/num.movie[1,i];
  }
  return (avg.rank);
}
avg.rank = rank_avg(d,num.movie);
avg.rank = data.frame(avg.rank);
names(avg.rank) = c(1:12);

#avg.spectator라는 이름의 행렬을 만들어서 각 월의 spectator의 평균을 계산하여 저장한다. 그 후 data.frame형으로 바꿔준다.
avg.spectator = matrix(0,ncol = 12,nrow=1);
spectator_avg = function(d,num.movie){
  avg.spectator = matrix(0,ncol = 12,nrow=1);
  sum = matrix(0,ncol = 12,nrow=1);
  for(i in 1:400){
    for(j in 1:12){
      if(d[i,3] == j){
        sum[1,j] = sum[1,j] + d[i,5];
      }
    }
  }
  for(i in 1:12){
    avg.spectator[1,i] = sum[1,i]/num.movie[1,i];
  }
  return (avg.spectator);
}
avg.spectator = spectator_avg(d,num.movie);
avg.spectator = data.frame(avg.spectator);
names(avg.spectator) = c(1:12);

#num.company라는 이름의 data.frame에 각 배급사별 영화의 숫자를 계산하여 저장한다.
num.company = data.frame(cjent=0, cjenm=0,disney=0,fox=0, lotte=0, next_=0, showbox=0, sony=0, universal=0, warner=0, etc=0)
company_count = function(d,num.company){
  num.company = data.frame(cjent=0, cjenm=0,disney=0,fox=0, lotte=0, next_=0, showbox=0, sony=0, universal=0, warner=0, etc=0)
  for(i in 1:400){
    if(d[i,8] == "cj ent"){
      num.company[1,1]=num.company[1,1]+1;
    }else if(d[i,8] == "cjenm"){
      num.company[1,2]=num.company[1,2]+1;
    }else if(d[i,8] == "disney"){
      num.company[1,3]=num.company[1,3]+1;
    }else if(d[i,8] == "fox"){
      num.company[1,4]=num.company[1,4]+1;
    }else if(d[i,8] == "lotte"){
      num.company[1,5]=num.company[1,5]+1;
    }else if(d[i,8] == "next"){
      num.company[1,6]=num.company[1,6]+1;
    }else if(d[i,8] == "showbox"){
      num.company[1,7]=num.company[1,7]+1;
    }else if(d[i,8] == "sony"){
      num.company[1,8]=num.company[1,8]+1;
    }else if(d[i,8] == "universal"){
      num.company[1,9]=num.company[1,9]+1;
    }else if(d[i,8] == "warner"){
      num.company[1,10]=num.company[1,10]+1;
    }else {
      num.company[1,11]=num.company[1,11]+1;
    }
  } 
  return(num.company)
}
num.company=company_count(d,num.company)

#sum.sales.company라는 이름의 data.frame에 각 배급사별 매출액의 총합을 계산하여 저장한다.
sum.sales.company = data.frame(cjent=0, cjenm=0,disney=0,fox=0, lotte=0, next_=0, showbox=0, sony=0, universal=0, warner=0, etc=0)
company_sales_sum = function(d,sum.sales.company){
  sum.sales.company = data.frame(cjent=0, cjenm=0,disney=0,fox=0, lotte=0, next_=0, showbox=0, sony=0, universal=0, warner=0, etc=0)
  for(i in 1:400){
    if(d[i,8] == "cj ent"){
      sum.sales.company[1,1]=sum.sales.company[1,1]+d[i,4];
    }else if(d[i,8] == "cjenm"){
      sum.sales.company[1,2]=sum.sales.company[1,2]+d[i,4];
    }else if(d[i,8] == "disney"){
      sum.sales.company[1,3]=sum.sales.company[1,3]+d[i,4];
    }else if(d[i,8] == "fox"){
      sum.sales.company[1,4]=sum.sales.company[1,4]+d[i,4];
    }else if(d[i,8] == "lotte"){
      sum.sales.company[1,5]=sum.sales.company[1,5]+d[i,4];
    }else if(d[i,8] == "next"){
      sum.sales.company[1,6]=sum.sales.company[1,6]+d[i,4];
    }else if(d[i,8] == "showbox"){
      sum.sales.company[1,7]=sum.sales.company[1,7]+d[i,4];
    }else if(d[i,8] == "sony"){
      sum.sales.company[1,8]=sum.sales.company[1,8]+d[i,4];
    }else if(d[i,8] == "universal"){
      sum.sales.company[1,9]=sum.sales.company[1,9]+d[i,4];
    }else if(d[i,8] == "warner"){
      sum.sales.company[1,10]=sum.sales.company[1,10]+d[i,4];
    }else {
      sum.sales.company[1,11]=sum.sales.company[1,11]+d[i,4];
    }
  } 
  return(sum.sales.company)
}
sum.sales.company=company_sales_sum(d,sum.sales.company)

#sum.spectator.company라는 이름의 data.frame에 각 배급사별 관객의 총합을 계산하여 저장한다.
sum.spectator.company = data.frame(cjent=0, cjenm=0,disney=0,fox=0, lotte=0, next_=0, showbox=0, sony=0, universal=0, warner=0, etc=0)
company_spectator_sum = function(d,sum.spectator.company){
  sum.spectator.company = data.frame(cjent=0, cjenm=0,disney=0,fox=0, lotte=0, next_=0, showbox=0, sony=0, universal=0, warner=0, etc=0)
  for(i in 1:400){
    if(d[i,8] == "cj ent"){
      sum.spectator.company[1,1]=sum.spectator.company[1,1]+d[i,5];
    }else if(d[i,8] == "cjenm"){
      sum.spectator.company[1,2]=sum.spectator.company[1,2]+d[i,5];
    }else if(d[i,8] == "disney"){
      sum.spectator.company[1,3]=sum.spectator.company[1,3]+d[i,5];
    }else if(d[i,8] == "fox"){
      sum.spectator.company[1,4]=sum.spectator.company[1,4]+d[i,5];
    }else if(d[i,8] == "lotte"){
      sum.spectator.company[1,5]=sum.spectator.company[1,5]+d[i,5];
    }else if(d[i,8] == "next"){
      sum.spectator.company[1,6]=sum.spectator.company[1,6]+d[i,5];
    }else if(d[i,8] == "showbox"){
      sum.spectator.company[1,7]=sum.spectator.company[1,7]+d[i,5];
    }else if(d[i,8] == "sony"){
      sum.spectator.company[1,8]=sum.spectator.company[1,8]+d[i,5];
    }else if(d[i,8] == "universal"){
      sum.spectator.company[1,9]=sum.spectator.company[1,9]+d[i,5];
    }else if(d[i,8] == "warner"){
      sum.spectator.company[1,10]=sum.spectator.company[1,10]+d[i,5];
    }else {
      sum.spectator.company[1,11]=sum.spectator.company[1,11]+d[i,5];
    }
  } 
  return(sum.spectator.company)
}
sum.spectator.company=company_spectator_sum(d,sum.spectator.company)

#avg.sales.company라는 이름의 data.frame에 각 배급사별 매출액의 평균을 계산하여 저장한다.
avg.sales.company = data.frame(cjent=0, cjenm=0,disney=0,fox=0, lotte=0, next_=0, showbox=0, sony=0, universal=0, warner=0, etc=0)
company_sales_avg = function(sum.sales.company, avg.sales.company){
  avg.sales.company = data.frame(cjent=0, cjenm=0,disney=0,fox=0, lotte=0, next_=0, showbox=0, sony=0, universal=0, warner=0, etc=0)
  for(i in 1:11){
    avg.sales.company[1,i]=sum.sales.company[1,i]/num.company[1,i];
  } 
  return(avg.sales.company)
}
avg.sales.company=company_sales_avg(sum.sales.company,avg.sales.company)

#avg.spectator.company라는 이름의 data.frame에 각 배급사별 관객의 평균을 계산하여 저장한다.
avg.spectator.company = data.frame(cjent=0, cjenm=0,disney=0,fox=0, lotte=0, next_=0, showbox=0, sony=0, universal=0, warner=0, etc=0)
company_spectator_avg = function(sum.spectator.company, avg.spectator.company){
  avg.spectator.company = data.frame(cjent=0, cjenm=0,disney=0,fox=0, lotte=0, next_=0, showbox=0, sony=0, universal=0, warner=0, etc=0)
  for(i in 1:11){
    avg.spectator.company[1,i]=sum.spectator.company[1,i]/num.company[1,i];
  } 
  return(avg.spectator.company)
}
avg.spectator.company=company_spectator_avg(sum.spectator.company,avg.spectator.company)

#top_sales_company명에 각각의 company중 sales가 높은 순으로 10개를 뽑아내 data.frame형태로 저장한다.
top_sales_cjent = subset(d, d$company=="cj ent")
top_sales_cjenm = subset(d, d$company=="cjenm")
top_sales_disney = subset(d, d$company=="disney")
top_sales_fox = subset(d, d$company=="fox")
top_sales_lotte = subset(d, d$company=="lotte")
top_sales_next = subset(d, d$company=="next")
top_sales_showbox = subset(d, d$company=="showbox")
top_sales_sony = subset(d, d$company=="sony")
top_sales_universal = subset(d, d$company=="universal")
top_sales_warner = subset(d, d$company=="warner")
top_sales_etc = subset(d, d$company == levels(d$company)[5] | d$company == levels(d$company)[1])
top10_sales = function(data){
  result = data[order(data$sales,decreasing = T)[1],]
  for(i in 2:10){
    result = rbind(result,data[order(data$sales,decreasing = T)[i],])
  }
  return(result)
}
top_sales_cjent = top10_sales(top_sales_cjent)
top_sales_cjenm = top10_sales(top_sales_cjenm)
top_sales_disney = top10_sales(top_sales_disney)
top_sales_fox = top10_sales(top_sales_fox)
top_sales_lotte = top10_sales(top_sales_lotte)
top_sales_next = top10_sales(top_sales_next)
top_sales_showbox = top10_sales(top_sales_showbox)
top_sales_sony = top10_sales(top_sales_sony)
top_sales_universal = top10_sales(top_sales_universal)
top_sales_warner = top10_sales(top_sales_warner)
top_sales_etc = top10_sales(top_sales_etc)

#top_spec_company명에 각각의 company중 spectator가 높은 순으로 10개를 뽑아내 data.frame형태로 저장한다.
top_spec_cjent = subset(d, d$company=="cj ent")
top_spec_cjenm = subset(d, d$company=="cjenm")
top_spec_disney = subset(d, d$company=="disney")
top_spec_fox = subset(d, d$company=="fox")
top_spec_lotte = subset(d, d$company=="lotte")
top_spec_next = subset(d, d$company=="next")
top_spec_showbox = subset(d, d$company=="showbox")
top_spec_sony = subset(d, d$company=="sony")
top_spec_universal = subset(d, d$company=="universal")
top_spec_warner = subset(d, d$company=="warner")
top_spec_etc = subset(d, d$company == levels(d$company)[5] | d$company == levels(d$company)[1])
top10_spec = function(data){
  result = data[order(data$spectator,decreasing = T)[1],]
  for(i in 2:10){
    result = rbind(result,data[order(data$spectator,decreasing = T)[i],])
  }
  return(result)
}
top_spec_cjent = top10_spec(top_spec_cjent)
top_spec_cjenm = top10_spec(top_spec_cjenm)
top_spec_disney = top10_spec(top_spec_disney)
top_spec_fox = top10_spec(top_spec_fox)
top_spec_lotte = top10_spec(top_spec_lotte)
top_spec_next = top10_spec(top_spec_next)
top_spec_showbox = top10_spec(top_spec_showbox)
top_spec_sony = top10_spec(top_spec_sony)
top_spec_universal = top10_spec(top_spec_universal)
top_spec_warner = top10_spec(top_spec_warner)
top_spec_etc = top10_spec(top_spec_etc)

#각 배급사의 상위 10개의 sales들의 평균을 구하여 top_sales_avg라는 data.frame에 저장한다.
top_sales_avg = data.frame(cjent=0, cjenm=0,disney=0,fox=0, lotte=0, next_=0, showbox=0, sony=0, universal=0, warner=0, etc=0)
top_sa_avg = function(data){
  sum=0
  for(i in 1:10){
    sum = sum + data[i,4]
  }
  result = sum/10
  return(result)
}
top_sales_avg[1,1] = top_sa_avg(top_sales_cjent)
top_sales_avg[1,2] = top_sa_avg(top_sales_cjenm)
top_sales_avg[1,3] = top_sa_avg(top_sales_disney)
top_sales_avg[1,4] = top_sa_avg(top_sales_fox)
top_sales_avg[1,5] = top_sa_avg(top_sales_lotte)
top_sales_avg[1,6] = top_sa_avg(top_sales_next)
top_sales_avg[1,7] = top_sa_avg(top_sales_showbox)
top_sales_avg[1,8] = top_sa_avg(top_sales_sony)
top_sales_avg[1,9] = top_sa_avg(top_sales_universal)
top_sales_avg[1,10] = top_sa_avg(top_sales_warner)
top_sales_avg[1,11] = top_sa_avg(top_sales_etc)

#각 배급사의 상위 10개의 spectator들의 평균을 구하여 top_spec_avg라는 data.frame에 저장한다.
top_spec_avg = data.frame(cjent=0, cjenm=0,disney=0,fox=0, lotte=0, next_=0, showbox=0, sony=0, universal=0, warner=0, etc=0)
top_sp_avg = function(data){
  sum=0
  for(i in 1:10){
    sum = sum + data[i,5]
  }
  result = sum/10
  return(result)
}
top_spec_avg[1,1] = top_sp_avg(top_spec_cjent)
top_spec_avg[1,2] = top_sp_avg(top_spec_cjenm)
top_spec_avg[1,3] = top_sp_avg(top_spec_disney)
top_spec_avg[1,4] = top_sp_avg(top_spec_fox)
top_spec_avg[1,5] = top_sp_avg(top_spec_lotte)
top_spec_avg[1,6] = top_sp_avg(top_spec_next)
top_spec_avg[1,7] = top_sp_avg(top_spec_showbox)
top_spec_avg[1,8] = top_sp_avg(top_spec_sony)
top_spec_avg[1,9] = top_sp_avg(top_spec_universal)
top_spec_avg[1,10] = top_sp_avg(top_spec_warner)
top_spec_avg[1,11] = top_sp_avg(top_spec_etc)
top_spec_avg

##############################################################################################################################
#Visualization

#ggplot2설치
if(!require(ggplot2)){
  install.packages("ggplot2")
  require(ggplot2)
}

#계절별 나오는 영화의 수
p = ggplot(d, aes(season,fill=season))
p = p + geom_bar(width=.5)
p

opar = par(mfrow=c(2,2))
#월별 나오는 영화의 수
plot(c(1:12),num.movie,xlab = "Month",ylab = "The number of movie",ylim=c(10,70))

#월별 나오는 영화들의 랭크의 평균
plot(c(1:12),avg.rank,xlab = "Month",ylab = "The average of rank",ylim=c(120,280))

#월별 나오는 영화들의 관객수 평균
plot(c(1:12),avg.spectator,xlab = "Month",ylab = "The average of spectator",ylim=c(2000000,4600000))

#Screen과 Spectator의 관계
p = ggplot(data=d, aes(x=screen,y=spectator))
p = p + geom_point()
p
opar = par(mfrow=c(1,1))
plot(d$screen,d$spectator,xlab = "Screen",ylab = "Spectator")
abline(a=93451,b=5399.5,col="red")

#Showed와 Spectator의 관계
plot(d$showed, d$spectator, xlab = "Showed", ylab = "Spectator")
abline(a=-941120, b=67.143, col="red")

#배급사별 출시한 영화의 수
p = ggplot(data = d, aes(company, fill=company))
p = p + geom_bar(width=.5)
p

#각 계절별 배급사에 따라 출시된 영화의 수
p = ggplot(data = d, aes(company, fill=company))
p = p + geom_bar(width=.5)
p = p + facet_grid(.~season)
p

#배급사별 매출 또는 관객수의 총합이나 평균을 막대그래프로 표현
name = matrix(names(sum.sales.company))
name
temp = t(matrix(sum.sales.company))
barplot(temp,width=1, names.arg = name[1:11,1])
title(main = "배급사별 매출의 총합")
temp = t(matrix(sum.spectator.company))
barplot(temp,width=1, names.arg = name[1:11,1])
title(main = "배급사별 관객수의 총합")
temp = t(matrix(avg.sales.company))
barplot(temp,width=1, names.arg = name[1:11,1])
title(main = "배급사별 매출의 평균")
temp = t(matrix(avg.spectator.company))
barplot(temp,width=1, names.arg = name[1:11,1])
title(main = "배급사별 관객수의 평균")
temp = t(matrix(top_sales_avg))
barplot(temp,width=1, names.arg = name[1:11,1])
title(main = "배급사별 상위 10위 매출의 평균")
temp = t(matrix(top_spec_avg))
barplot(temp,width=1, names.arg = name[1:11,1])
title(main = "배급사별 상위 10위 관객수의 평균")

##############################################################################################################################
#아직 쓸지 안 쓸지 모름

            #계절별로 subset을 이용해서 따로 잘라 data.frame으로 저장한다.
            spring = subset(d, d$month<=5 & d$month>=3)
            summer = subset(d, d$month<=8 & d$month>=6)
            fall = subset(d, d$month<=11 & d$month>=9)
            winter = subset(d, d$month<=2 | d$month>=12)
            
            #계절별로 평균값을 계산한다.
            d_spring = sapply(spring[,4:7], mean)
            d_spring = as.data.frame(t(d_spring))
            d_summer = sapply(summer[,4:7], mean)
            d_summer = as.data.frame(t(d_summer))
            d_fall = sapply(fall[,4:7], mean)
            d_fall = as.data.frame(t(d_fall))
            d_winter = sapply(winter[,4:7], mean)
            d_winter = as.data.frame(t(d_winter))
            
            #각 계절별 관객수와 매출액의 관계
            opar = par(mfrow=c(2,2))
            plot(spring$spectator, spring$money)
            plot(summer$spectator, summer$money)
            plot(fall$spectator, fall$money)
            plot(winter$spectator, winter$money)
            