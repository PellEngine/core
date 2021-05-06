const path = require('path');
const fs = require('fs');
const os = require('os');
const exec_process = require('child_process').exec;

// Project variables
const directory = process.cwd();

// Read config and cache
const libConfigPath = path.join(directory, 'pellengine.json');
if(!fs.existsSync(libConfigPath)) {
  return;
}
const libConfig = JSON.parse(fs.readFileSync(libConfigPath));

const libCachePath = path.join(directory, '.pellengine-cache.json');
if(!fs.existsSync(libCachePath)) {
  return;
}
const libCache = JSON.parse(fs.readFileSync(libCachePath));

function exec(cmd) {
  return new Promise((resolve, reject) => {
    exec_process(cmd, (error, stdout, stderr) => {
      if(error) {
        console.log(error);
      }

      resolve(stdout ? stdout : stderr);
    });
  });
}

const clean = () => {
  const buildDir = path.join(directory, 'build', 'shaders');
  if(fs.existsSync(buildDir)) fs.rmdirSync(buildDir, { recursive: true });
}

const compileShaders = async () => {
  const buildDir = path.join(directory, 'build', 'shaders');

  if(!fs.existsSync(buildDir)) {
    fs.mkdirSync(buildDir);
  }

  const ndk = libCache.androidNdk;
  const platform = os.platform();
  let osName;

  if(platform === 'linux') {
    osName = 'linux-x86_64';
  } else if(platform === 'win32') {
    osName = 'windows-x86_64';
  } else if(platform === 'darwin') {
    osName = 'darwin-x86_64';
  }

  const shaderCompiler = path.join(ndk, 'shader-tools', osName, 'glslc');
  const shaderFiles = fs.readdirSync(path.join(directory, 'shaders'));

  for(let file of shaderFiles) {
    console.log('Compiling shader ' + file);
    console.log();

    let command = `${shaderCompiler} ${path.join(directory, 'shaders', file).toString()} -o ${path.join(buildDir, file + '.spv').toString()} -fentry-point=main`;
    console.log(await exec(command));
    console.log();
  }
};

async function performBuild() {
  clean();
  await compileShaders();
}

performBuild();