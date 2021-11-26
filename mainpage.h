const char mainPage[] PROGMEM = R"=====(
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1" />
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<link rel="icon" href="data:,">
<title>Yaesu FT757gx Remote Control</title>
<style>
  body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; text-align: center;}
  #vfoFreq {background-color: #000000; color: #50de95; text-align: center; width: 300px; border: none; height: 80px; font-weight: bold; line-height: 80px; font-size: 3.2em; box-shadow: inset 2px 2px 3px rgba(255, 255, 255, .6), inset -2px -2px 3px rgba(255, 255, 255, .6); }
  #stepTable { margin-left: auto; margin-right: auto; margin-top: 20px; height: 30px; width: 300px; display: table; border-collapse: separate; border-spacing: 5px;}
  #stepTable > div {display: table-cell; vertical-align: middle; background-color: #555555; padding: 8px; height: 30px; width: 50px; Color: #eeeeee; font-weight: bold; cursor: pointer; box-shadow: inset 2px 2px 3px rgba(255, 255, 255, .6),
                inset -2px -2px 3px rgba(0, 0, 0, .6); text-shadow: 1px 1px 1px #000;}
  #stepTable > div:active { box-shadow: inset -2px -2px 3px rgba(255, 255, 255, .6), inset 2px 2px 3px rgba(0, 0, 0, .6);}
  #freqTable { display: table; margin-left: auto; margin-right: auto; border-spacing: 5px; border-collapse: separate; }
  #freqtable > div > div { display: table-row;}
  #freqTable > div > div > div { display: table-cell; background-color: #555555; padding: 10px; height: 50px; width: 110px; Color: #eeeeee; font-weight: bold; vertical-align: middle; cursor: pointer; box-shadow: inset 2px 2px 3px rgba(255, 255, 255, .6), inset -2px -2px 3px rgba(0, 0, 0, .6); text-shadow: 1px 1px 1px #000;}
  #freqTable > div > div > div:active { box-shadow: inset -2px -2px 3px rgba(255, 255, 255, .6), inset 2px 2px 3px rgba(0, 0, 0, .6);}
</style>
<script type="text/javascript">
var curFreq='';
window.addEventListener("load", function() {
  var freqField = document.getElementById("vfoFreq");
  curFreq = freqField.value;
  function displayFreq(freq) {
    freqField.value=freq;
  }
  function sendFreq(freq) {
    clearTimeout(autoRefresh);
    var XHR = new XMLHttpRequest();
    
    XHR.addEventListener("load", function (event) {
      if (event.target.responseText == "OK") {
        displayFreq(freq);
        curFreq=freq;
        autoRefresh = setTimeout(getFreq, 15000);
      } else {
        displayFreq(curFreq);
      }
      freqField.style.color='#50de95'
    });
    
    XHR.addEventListener("error", function (event) {
      freqField.style.color='#de5095';
    }); 
    
    freqField.style.color='#dedede'
    XHR.open("GET", "/freq?freq="+freq);
    XHR.send();
  }

  function getFreq() {
    clearTimeout(autoRefresh);
    var XHR = new XMLHttpRequest();
    XHR.addEventListener("load", function (event) {
      var freq = event.target.responseText;
      if (freq != curFreq) {
        curFreq = freq;
        displayFreq(freq);
        freqField.style.color='#50de95';
      }
      autoRefresh = setTimeout(getFreq, 10000);
    });

    XHR.addEventListener("error", function (event) {
      freqField.style.color='#de5095';
    });
    XHR.open("GET", "/freq");
    XHR.send();
  }

  var freqForm = document.getElementById("freqForm");
  freqForm.addEventListener("submit", function (event) {
    event.preventDefault();
    sendFreq(freqField.value);
  });

  
  freqForm.addEventListener("change", function (event) {
    freqField.style.color='#dedede';
  });

  var tf = document.querySelectorAll('#freqTable div div div');
  tf.forEach(function(item) {
    item.addEventListener('click', function(e) {
      var t = e.currentTarget;
        sendFreq(t.id);
    });
  });
    
  var ts = document.querySelectorAll('#stepTable div');
  ts.forEach(function(item) {
  item.addEventListener('click', function(e) {
    var t = e.currentTarget;
    var step = parseInt(t.id, 10);
    var freq = parseInt(curFreq);
    if (isNaN(step) || isNaN(freq)) {return;}
    var newFreq = freq + step;
    sendFreq(newFreq);
  });
  });

  var autoRefresh = setTimeout(getFreq, 10000);
});
</script>
</head>
<body>
 <h1>FT-757gx Remote VFO</h1>
 <form id="freqForm">
 <input type="text" id="vfoFreq" value="%FREQ%">
 </form>
 <div id="stepTable">
 <div id="-5000">-5K</div><div id="-1000">-1K</div><div id="-100">-100</div>
 <div id="-10">-10</div><div id="10">+10</div><div id="100">+100</div>
 <div id="1000">+1K</div><div id="5000">+5K</div></div>
 <div id="freqTable">
 <div><div><div id="27555000">27.555</div><div id="27395000">27.395</div><div id="27945000">27.945</div></div></div>
 <div><div><div id="27405000">27.405</div><div id="27305000">27.305</div><div id="27045000">27.045</div></div></div>
 </div>
</body>
</html>
)=====";
