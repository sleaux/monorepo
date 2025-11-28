request = new XMLHttpRequest();
request.open('GET', '../../bazel-out/k8-fastbuild/bin/modules/ziggy/lib_wasm');
request.responseType = 'arraybuffer';
request.send();

request.onload = function() {
  var bytes = request.response;
  WebAssembly.instantiate(bytes, {
    env: {}
  }).then(result => {
    var add_two = result.instance.exports.add_two;
    console.log(add_two(3, 5));
  });
};
