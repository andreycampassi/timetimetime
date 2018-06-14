var url = 'mqtt://testebatata123:teste123@broker.shiftr.io';          //token KEY and SECRET from shiftr.io
var client = mqtt.connect(url, {clientId: 'web-page'});  //arbitrary identification
var QoS = 1;

var monalisa_element = document.getElementById("monalisa");
var html_element = document.getElementsByTagName("html")[0];

var vidContainer = document.getElementById("player");
var videoBom = "video/video1.mp4";
var videoRuim = "video/video2.mp4";
var legenda = document.getElementById("legenda");





var red = 255;
var green = 255;
var blue = 255;

var red_element = document.getElementById("red");
var green_element = document.getElementById("green");
var blue_element = document.getElementById("blue");

function updateBom(){
    var track;
    vidContainer.src = videoBom;
    track = vidContainer.addTextTrack("video/video1.vtt");
    track.mode = "showing";

    vidContainer.play();
    // Da play no video
    // Leds da cidade
}

function updateRuim(){
    var track;
    vidContainer.src = videoRuim;
    vidContainer.addTextTrack("video/video2.vtt");
    track.mode = "showing";
    vidContainer.play();
    // Da play no video ruim
    // Buzzer
    // Alertas Ruins
}

function updateRed(value)
{
    red = value;
    updateRGB();
}

function updateGreen(value)
{
    green = value;
    updateRGB();
}

function updateBlue(value)
{
    red = blue;
    updateRGB();
}

function updateRGB()
{
    red = red_element.value;
    green = green_element.value;
    blue = blue_element.value;
    html_element.style.background = "rgb(" + red + "," + green + "," + blue + ")";
    publishTopic('/red', red);
    publishTopic('/green', green);
    publishTopic('/blue', blue);
}

client.on('connect', function()
{
    console.log("MQTT Connected!");
    client.subscribe('/vibration', { qos: QoS });
});

client.on('error', function(e)
{
    console.log(e);
});

function publishTopic(topic, valor)
{
    client.publish(topic, valor, { qos: QoS, retain: false });
    console.log("Published: " + topic + ": " + valor);
}

client.on('message', function(topic, message)
{
    console.log('New message:', topic, message.toString());

    if (topic == "/vibration")
    {
        if (parseInt(message) == 1)
        {
            monalisa_element.classList.add("vibrating");
        }
        else
        {
            monalisa_element.classList.remove("vibrating");
        }
    }

});
