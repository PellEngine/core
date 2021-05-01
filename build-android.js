const path = require('path');
const fs = require('fs');
const os = require('os');
const exec_process = require('child_process').exec;
 

// Project variables
const directory = process.cwd();
const androidVersion = 29;

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
  const buildDir = path.join(directory, 'build');
  if(fs.existsSync(buildDir)) fs.rmdirSync(buildDir, { recursive: true });
};

const buildLib = async () => {
  const buildDir = path.join(directory, 'build', 'android');

  const architectures = [
    'arm64-v8a',
    'armeabi-v7a',
    'x86',
    'x86_64'
  ];
  
  const libFolders = {
    'arm64-v8a': 'aarch64-linux-android/' + androidVersion,
    'armeabi-v7a': 'armv7a-linux-androideabi/' + androidVersion,
    'x86': 'i686-linux-android/' + androidVersion,
    'x86_64': 'x86_64-linux-android/' + androidVersion
  };

  const compilers = {
    'arm64-v8a': `aarch64-linux-android${androidVersion}-clang++`,
    'armeabi-v7a': `armv7a-linux-androideabi${androidVersion}-clang++`,
    'x86': `i686-linux-android${androidVersion}-clang++`,
    'x86_64': `x86_64-linux-android${androidVersion}-clang++`
  };

  console.log('Set up build directory...');
  architectures.map(arch => path.join(buildDir, 'lib', arch)).forEach(buildFolder => fs.mkdirSync(buildFolder, { recursive: true }));
  architectures.map(arch => path.join(buildDir, 'obj', arch)).forEach(objectFolder => fs.mkdirSync(objectFolder, { recursive: true }));

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

  const cFlags = [
    '-fpic',
    '-Os',
    '-Wall',
    '-DANDROID',
    '-DANDROID_FULLSCREEN',
    `-I${path.join(ndk, 'sysroot', 'usr', 'include').toString()}`,
    `-I${path.join(ndk, 'sysroot', 'usr', 'include', 'android').toString()}`,
    `-I${path.join(ndk, 'toolchains', 'llvm', 'prebuilt', osName, 'sysroot', 'usr', 'include', 'android').toString()}`,
    `-I${path.join(directory, 'src').toString()}`,
    '-c',
    '-D DEBUG',
    '-D PLATFORM="ANDROID"',
    '-std=c++17'
  ];

  const archFlags = {
    'arm64-v8a': [
      '-m64'
    ],
    'armeabi-v7a': [
      '-mfloat-abi=softfp',
      '-m32'
    ],
    'x86': [
      '-march=i686',
      '-mtune=intel',
      '-mssse3',
      '-mfpmath=sse',
      '-m32'
    ],
    'x86_64': [
      '-march=x86-64',
      '-msse4.2',
      '-mpopcnt',
      '-m64',
      '-mtune=intel'
    ]
  };

  const ldFlags = [
    '-shared',
    '-s',
    '-lm',
    '-landroid',
    '-llog'
  ];

  const compilersRoot = path.join(ndk, 'toolchains', 'llvm', 'prebuilt', osName, 'bin');

  for(let arch of architectures) {
    console.log();
    console.log('Building for ' + arch);

    const compilerPath = path.join(compilersRoot, compilers[arch]);
    if(!fs.existsSync(compilerPath)) {
      console.log(`Compiler for ${arch} was not found.`);
      return;
    }

    console.log('Compiling...');
    console.log();

    const srcs = [
      ...libConfig.src,
      ...libConfig.androidSrc
    ];

    for(let src of srcs) {
      console.log('Compiling ' + src);

      const objectPath = src.substr(0, src.lastIndexOf('.')) + '.o';
      fs.mkdirSync(path.dirname(path.join(buildDir, 'obj', arch, src)), { recursive: true });

      let command = `${compilerPath} ${cFlags.join(' ')} ${archFlags[arch].join(' ')} `;
      command += `-o ${path.join(buildDir, 'obj', arch, objectPath)} `;
      command += `${src}`;
      
      console.log(await exec(command));
      console.log();
    }

    console.log('Linking for ' + arch);
    let command = `${compilerPath} ${ldFlags.join(' ')} `;
    command += `-L${path.join(ndk, 'toolchains', 'llvm', 'prebuilt', osName, 'sysroot', 'usr', 'lib', libFolders[arch]).toString()} `;
    command += `-o ${path.join(buildDir, 'lib', arch, `lib${libConfig.name}.so`).toString()} `
    command += `${srcs.map(src => {
      const objectPath = src.substr(0, src.lastIndexOf('.')) + '.o';
      return path.join(buildDir, 'obj', arch, objectPath).toString()
    }).join(' ')}`;

    console.log(await exec(command));
  }
}

async function performBuild() {
  clean();
  await buildLib();
}

performBuild();