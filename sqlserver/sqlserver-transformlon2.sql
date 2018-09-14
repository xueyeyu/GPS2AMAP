create function transformlon2(@lon float,@lat float) 
returns float 
as
begin
declare @marslon float ,@marslat float
declare @a float =6378245.0 ,@ee float = 0.00669342162296594323 ,@pi float =3.14159265358979324
declare @dlon float , @dlat float ,@radLat float , @magic float , @sqrtMagic float
declare @mglat float ,@mglon float 
declare @hid float =597037492 ,@x int 

		if (@lon!=0 and @lat != 0)
		begin
			set @dlat=dbo.transformLat(@lon-105.0,@lat-35.0)
			set @dlon=dbo.transformLon(@lon-105.0,@lat-35.0)
			set @radLat=@lat/180.0*@pi
			set @magic = sin(@radLat)
			set @magic = 1-@ee*@magic*@magic
			set @sqrtMagic = sqrt (@magic)
			set @dlat = (@dlat * 180.0) / ((@a * (1 - @ee)) / (@magic * @sqrtMagic) * @pi);
			set @dlon = (@dlon * 180.0) / (@a / @sqrtMagic * cos(@radLat) * @pi);
			set @mglat=@lat+@dlat
			set @mglon=@lon+@dlon
			end
	return @mglon
end
