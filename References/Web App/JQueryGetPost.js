/**
 * JQuery Get and Post example with Photons.
 * Author: Daiwei Chen
 */

// Here's a get example
function fetch(){
  //getRec();
  let url = "https://api.particle.io/v1/devices/"+device_id+"/rec?access_token="+access_t;
  $.get(url, function(data, status){
    $('#rec').html('Most Recent: ' + data['result'])
  });
}

// Here is a post example
function reset(){
  const url = "https://api.particle.io/v1/devices/"+device_id+"/reset";
  const post_fields = {'access_token': access_t, 'args': 'message'};
  $.post(url, post_fields, function(data, status){
    console.log(data);
    fetch();
  });
}