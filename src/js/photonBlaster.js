const device_id = "400052000d51353532343635";
const access_t = "4938386ce157fb73505b2623c6c421cb3712ab1d";
var r;
var g;
var b;
function func(){
	setInterval(function(){
  //getRed();
  url = "https://api.particle.io/v1/devices/"+device_id+"/red?access_token="+access_t;
  $.get(url, function(data, status){
    r = data['result']
	console.log(r)
  });

//getGreen();
  url = "https://api.particle.io/v1/devices/"+device_id+"/green?access_token="+access_t;
  $.get(url, function(data, status){
    g = data['result']
	console.log(g)
  });
 
//getBlue();
  url = "https://api.particle.io/v1/devices/"+device_id+"/blue?access_token="+access_t;
  $.get(url, function(data, status){
    b = data['result']
	console.log(b)
  });
	$("#color").text("rgb("+r+","+g+","+b+")");
	$("#color").css("color","rgb("+r+","+g+","+b+")");
},500);
}
//sends rgb to photon
function send(){
  var first = ($('#box1').val());
  if (first > 256){
	first=256;
  }	  
  if (first < 0){
	first=0;
  }
  var second = ($('#box2').val());
  if (second > 256){
	second=256;
  }	  
  if (second < 0){
	second=0;
  }
  var third = ($('box3').val());
  if (third > 256){
	third=256;
  }	  
  if (third < 0){
	third=0;
  }
  var info = first + " " + second + " " + third;
  
  const url = "https://api.particle.io/v1/devices/"+device_id+"/fadeColor";
  const post_fields = {'access_token': access_t, 'args': info}
  $.post(url, post_fields, function(data, status){
    console.log(data);
  });
}
  function rainbow(){
  const url = "https://api.particle.io/v1/devices/"+device_id+"/rainbow";
  const post_fields = {'access_token': access_t, 'args': info}
  $.post(url, post_fields, function(data, status){
    console.log(data);
  }); 
  }
  function strobe(){
  const url = "https://api.particle.io/v1/devices/"+device_id+"/strobe";
  const post_fields = {'access_token': access_t, 'args': info}
  $.post(url, post_fields, function(data, status){
    console.log(data);
  }); 
  }
	  

