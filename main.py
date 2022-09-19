# This is a sample Python script.

# Press ⌃R to execute it or replace it with your code.
# Press Double ⇧ to search everywhere for classes, files, tool windows, actions, and settings.


# 🌏 经纬度海拔
# 时间：2022年9月18日 01:30:08
#
# 经纬度：北纬 32°12′74″ 东经 118°96′64″
# 海    拔：24.3米



import pysolar.solar as Sun
import time
import datetime
import requests
import csv
import json


def gd_map(addr):
    para = {'key': '1a8eba14b0ebee4924f398ceeb03448e',  # 高德Key
            'address': addr}  # 地址参数
    url = 'https://restapi.amap.com/v3/geocode/geo?'  # 高德地图地理编码API服务地址
    result = requests.get(url, para)  # GET方式请求
    result = result.json()
    lon_lat = result['geocodes'][0]['location']  # 获取返回参数geocodes中的location，即经纬度
    print(result)
    return lon_lat

print("请在下面的列表中选择地址格式\n1. 文字地址\n2. 度分秒格式的经纬度\n3.小数格式的经纬度\n输入数字编号以选择：")
chooseNum = int(input())


if chooseNum == 1:
    print("-------------文字地址模式----------------")
    address = input("请输入文字地址：")
    longLatPos = gd_map(addr = address)

    longtitude_srting,latitude_string=longLatPos.split(',',2)

    longtitude = float(longtitude_srting)
    latitude  = float(latitude_string)

    print(address + longLatPos)



elif chooseNum == 2:
    print("--------------度分秒经纬度格式---------------------")
    longtitude_deg = float(input("请输入经度：\n度："))
    longtitude_min = float(input("分："))
    longtitude_sec = float(input("秒："))
    longtitude = longtitude_deg + longtitude_min / 60 + longtitude_sec / 3600

    latitude_deg = float(input("请输入纬度：\n度："))
    latitude_min = float(input("分："))
    latitude_sec = float(input("秒："))
    latitude = latitude_deg + latitude_min / 60 + latitude_sec / 3600

elif chooseNum == 3:
    print("--------------小数经纬度格式---------------------")
    latitude = float(input("请输入纬度："))
    longtitude = float(input("请输入经度："))

print("最终地址的经纬度为：纬度：" + str(latitude) + "经度：" + str(longtitude))



print("\n--------------输入部分结束---------------------\n")




t0 = datetime.datetime.now(tz=datetime.timezone.utc)
second = int(t0.second)
minute = int(t0.minute)
hour = int(t0.hour)
while True:
    # second += 1
    # if second == 59:
    #     second = 0
    #     minute += 1
    # if minute == 59:
    #     minute = 0
    #     hour += 1
    # if Sun.get_altitude(latitude, longtitude, datetime.datetime(2022,9,18,hour,minute,second,tzinfo = datetime.timezone.utc)) >= 0:
    #
    #     print("Sunrise Time:" + str(hour) + ":" + str(minute) + ":" + str(second))
    #     quit()

    print("当前时间为：" + str(t0))
    print("该点处的太阳高度角为：" + str(Sun.get_altitude(latitude, longtitude, t0)))
    print("该点处的太阳方位角为：" + str(Sun.get_azimuth(latitude, longtitude, t0)))
    print("")
    time.sleep(1)




# See PyCharm help at https://www.jetbrains.com/help/pycharm/
