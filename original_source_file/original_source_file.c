//============================================================================
// Name        : Convert_cpp.cpp
// Author      : roger
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================


#include <math.h>
#include <stdlib.h>

#include <stdio.h>


const double pi = 3.14159265358979324;
	const double a = 6378245.0;
	const double ee = 0.00669342162296594323;
	const  double x_pi = 3.14159265358979324 * 3000.0 / 180.0;
	
	//判断是否在中国境内
	bool outOfChina(double lat, double lon)
	{
		if (lon < 72.004 || lon > 137.8347)
			return true;
		if (lat < 0.8293 || lat > 55.8271)
			return true;
		return false;
	}
	
	
	 double transformLat(double x, double y)
	{
		double ret = -100.0 + 2.0 * x + 3.0 * y + 0.2 * y * y + 0.1 * x * y + 0.2 * sqrt(abs(x));
		ret += (20.0 * sin(6.0 * x * pi) + 20.0 * sin(2.0 * x * pi)) * 2.0 / 3.0;
		ret += (20.0 * sin(y * pi) + 40.0 * sin(y / 3.0 * pi)) * 2.0 / 3.0;
		ret += (160.0 * sin(y / 12.0 * pi) + 320 * sin(y * pi / 30.0)) * 2.0 / 3.0;
		return ret;
	}
	
	
	 double transformLon(double x, double y)
	{
		double ret = 300.0 + x + 2.0 * y + 0.1 * x * x + 0.1 * x * y + 0.1 * sqrt(abs(x));
		ret += (20.0 * sin(6.0 * x * pi) + 20.0 * sin(2.0 * x * pi)) * 2.0 / 3.0;
		ret += (20.0 * sin(x * pi) + 40.0 * sin(x / 3.0 * pi)) * 2.0 / 3.0;
		ret += (150.0 * sin(x / 12.0 * pi) + 300.0 * sin(x / 30.0 * pi)) * 2.0 / 3.0;
		return ret;
	}

    /**
     * 地球坐标转换为火星坐标
     * World Geodetic System ==> Mars Geodetic System
     *
     * @param wgLat  地球坐标
     * @param wgLon
     *
     * mglat,mglon 火星坐标
     */
     void transform2Mars(double wgLat, double wgLon,double &mgLat,double &mgLon)
    {
        if (outOfChina(wgLat, wgLon))
        {
            mgLat  = wgLat;
            mgLon = wgLon;
            return ;
        }
        double dLat = transformLat(wgLon - 105.0, wgLat - 35.0);
        double dLon = transformLon(wgLon - 105.0, wgLat - 35.0);
        double radLat = wgLat / 180.0 * pi;
        double magic = sin(radLat);
        magic = 1 - ee * magic * magic;
        double sqrtMagic = sqrt(magic);
        dLat = (dLat * 180.0) / ((a * (1 - ee)) / (magic * sqrtMagic) * pi);
        dLon = (dLon * 180.0) / (a / sqrtMagic * cos(radLat) * pi);
        mgLat = wgLat + dLat;
        mgLon = wgLon + dLon;
		

    }

    /**
     * 火星坐标转换为百度坐标
     * @param gg_lat
     * @param gg_lon
     */
     void bd_encrypt(double gg_lat, double gg_lon,double &bd_lat,double & bd_lon)
    {
        double x = gg_lon, y = gg_lat;
        double z = sqrt(x * x + y * y) + 0.00002 * sin(y * x_pi);
        double theta = atan2(y, x) + 0.000003 * cos(x * x_pi);
        bd_lon = z * cos(theta) + 0.0065;
        bd_lat = z * sin(theta) + 0.006;

    }

    /**
     * 百度转火星
     * @param bd_lat
     * @param bd_lon
     */
     void bd_decrypt(double bd_lat, double bd_lon,double &gg_lat,double &gg_lon)
    {
        double x = bd_lon - 0.0065, y = bd_lat - 0.006;
        double z = sqrt(x * x + y * y) - 0.00002 * sin(y * x_pi);
        double theta = atan2(y, x) - 0.000003 * cos(x * x_pi);
        gg_lon = z * cos(theta);
        gg_lat = z * sin(theta);

    }



int main() {
	//double lat = 30.227607;
	//double lon = 120.036565;
	
	double lat,lon;
	printf("输入经纬度坐标 先输入lat后输入lon\n");
	scanf("%lf%lf",&lat,&lon);
	//真实的经纬度转化为百度地图上的经纬度，便于计算百度POI
	double marsLat = 0;
	double marsLon = 0;
	double resultLat = 0;
	double resultLon = 0;
	transform2Mars(lat,lon,marsLat,marsLon);
	printf("lat:%lf,lon:%lf\n",marsLat,marsLon);
	/*
	bd_encrypt(marsLat,marsLon,resultLat,resultLon);

	//30.2193456 120.0348264
	cout<<setprecision(10)<<resultLat<<" "<<setprecision(10)<<resultLon<<endl;
	*/
}
