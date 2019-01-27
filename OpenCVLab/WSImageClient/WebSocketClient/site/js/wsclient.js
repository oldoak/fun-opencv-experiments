if (window.File && window.FileReader && window.FileList && window.Blob) 
{
  //convert file size to readable format
  function fileSize(bytes, si) 
  {
    var thresh = si ? 1000 : 1024;
    if(bytes < thresh) return bytes + ' B';
    var units = si ? ['kB','MB','GB','TB','PB','EB','ZB','YB'] : 
                     ['KiB','MiB','GiB','TiB','PiB','EiB','ZiB','YiB'];
    var u = -1;
    do 
    {
      bytes /= thresh;
      ++u;
    } while(bytes >= thresh);

    return bytes.toFixed(1)+' '+units[u];
  }

  //send loaded image to server for processing
  function processImage(theImage)
  {
    if ("WebSocket" in window)
    {
      var image = document.getElementById("image");

      var ws = new WebSocket("ws://localhost:9980");
      ws.binaryType = "arraybuffer";
  
      ws.onopen = function()
      {
        ws.send(theImage);
      };
  
      ws.onmessage = function(evt) 
      { 
        console.log(evt.data);
        output.setAttribute("src", typeof(evt.data));
      };
  
      ws.onclose = function()
      { 
        alert("Connection is closed..."); 
      };

      window.onbeforeunload = function() 
      {
        //ws.onclose = function () {}; // disable onclose handler first
        ws.close()
      }
    }
    else
    {
      alert("WebSocket NOT supported by your Browser!");
    }
  }

  //main function is called when the input loads an image
  function main(event)
  {
    var fileList = event.target.files;
    var file = fileList[0];
    var objectURL = URL.createObjectURL(file);

    var canvas = document.getElementById("myCanvas");
    var ctx = canvas.getContext("2d");
    var img = new Image();
    img.onload = function () {
        canvas.width = img.width;
        canvas.height = img.height;
        ctx.drawImage(img, 0, 0);
        data = canvas.toDataURL(mime);
    };

    var reader = new FileReader();
    reader.onload = function(event)
    {
      document.getElementById("name").innerHTML = file.name;
      document.getElementById("size").innerHTML = fileSize(file.size, "MB");
      document.getElementById("type").innerHTML = file.type;
      mime = file.type;

      //img.onload called
      img.src = objectURL;

      var btn = document.createElement("BUTTON");
      var ttl = document.createTextNode("Process Image");
      btn.appendChild(ttl);
      btn.setAttribute("id", "websocket");
      btn.setAttribute("onclick", 
                       "javascript:processImage(data)");

      document.getElementById("inputimage").appendChild(btn);
    }
    
    //reader.onload called
    reader.readAsArrayBuffer(file);
  }
}
else
{
  alert('The File APIs are not fully supported in this browser.');
}
