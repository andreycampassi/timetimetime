// Estados da Página
const PLAYING_GOOD = 1;
const PLAYING_BAD = 2;
const NOTHING_PLAYING = 0;

var estadoAtual = NOTHING_PLAYING;

// Conexao com MQTT
var url = 'mqtt://testebatata123:teste123@broker.shiftr.io';          //token KEY and SECRET from shiftr.io
var client = mqtt.connect(url, {clientId: 'web-page'});  //arbitrary identification
var QoS = 1; // Quality of Service/TCP/UDP ?

// Legado
var html_element = document.getElementsByTagName("html")[0];

// Inicio Time
// Player de video da pagina web
var vidContainer = document.getElementById("player");
// Caminho para os Videos
var videoBom = "video/video1.mp4";
var videoRuim = "video/video2.mp4";

// Legendas dos videos
var trackBom = "video/video1.vtt";
var trackRuim = "video/video2.vtt";


function updatePlayer(videoURL, trackURL){
   vidContainer.src = videoURL;

   // Decide Legenda
   vidContainer.textTracks[trackURL==trackBom?0:1].mode = "showing";
   vidContainer.textTracks[trackURL==trackBom?1:0].mode = "hidden";

   //publishTopic("/startVideo"); // Avisa microcontrolador que o video comecou
   vidContainer.play();
}

function updateBom(){
    estadoAtual = PLAYING_GOOD;
    updatePlayer(videoBom, trackBom);
    // Da play no video
    // Leds da cidade
}

function updateRuim(){
    estadoAtual = PLAYING_BAD;
    updatePlayer(videoRuim, trackRuim);
    // Da play no video ruim
    // Buzzer
    // Alertas Ruins
}

function addMSGMQTT(){
    vidContainer.addEventListener('ended',endHandler,false);
    function endHandler(e) {
        publishTopic('/videoStatus', NOTHING_PLAYING);
        estadoAtual = NOTHING_PLAYING;
    }
    vidContainer.addEventListener('play',playHandler,false);
    function playHandler(e) {
        publishTopic('/videoStatus', estadoAtual);
    }
}

client.on('connect', function()
{
    console.log("MQTT Connected!");
    client.subscribe('/startVideo', { qos: QoS });
});

client.on('error', function(e)
{
    console.log(e);
});

function AnimaClick(){
    //algum feedback mostrando que teve interacao
}

function publishTopic(topic, valor)
{
    client.publish(topic, valor, { qos: QoS, retain: false });
    console.log("Published: " + topic + ": " + valor);
}

client.on('message', function(topic, message)
{
    console.log('New message:', topic, message.toString());

    if(topic == "/startVideo"){
        if(parseInt(message) == PLAYING_GOOD){
            if(estadoAtual == NOTHING_PLAYING){
                updateBom();
            } else {
                AnimaClick(estadoAtual, PLAYING_GOOD);
            }
        } else if(parseInt(message) == PLAYING_BAD) {
            if(estadoAtual == NOTHING_PLAYING || estadoAtual == PLAYING_GOOD){
                updateRuim();
            } else {
                AnimaClick(estadoAtual, PLAYING_BAD);
            }
        } else {
            // DeepTime?
        }
    }

});
