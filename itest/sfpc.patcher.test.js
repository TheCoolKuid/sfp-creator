
let fs = require('fs')
let crypt = require('crypto')
let path = require("path")
let child_proc = require("child_process")

jest.setTimeout(600000);


let cwd = path.join(process.cwd(), "tpf_temp");
let outer_dir = path.resolve(process.cwd(), "../");
let src_dir = path.join(cwd, "src");
let dest_dir = path.join(cwd, "dest");

let exe_dir = path.join(outer_dir, "ProgramRelease")
let configurator_exe = path.join(exe_dir, "Configurator.exe")

function RunProcesseGetRetCode(path, args, cwd) {
    return new Promise((resolve, reject) => {
        child_proc.spawn(path, args, {
            stdio: ['pipe', 'pipe', 'pipe'],
            cwd: cwd
        }).on('exit', (code, sig) => {
            resolve(code);
        })
    });    
}

test("build system", async () => {
    console.log(`cwd:${cwd}`)
    console.log(`src_dir:${src_dir}`)
    console.log(`dest_dir:${dest_dir}`)
    console.log(`exe_dir:${exe_dir}`)
    console.log(`configurator_exe:${configurator_exe}`)

    expect(await RunProcesseGetRetCode("Powershell", ["-File", path.join(outer_dir, "release.ps1")], outer_dir)).toBe(0);

    try{
        await fs.promises.access(configurator_exe);
    }catch(err) {
        expect(err).toBe(0);
    }
})

describe('test patcher functionality', () => {
    beforeAll( async () => {
        await fs.promises.mkdir(cwd);
        await fs.promises.mkdir(src_dir);
        await fs.promises.mkdir(dest_dir);
    });

    test("add files in empty dir", async () => {
               
        let config = {
            executable_name : "patche",
            add_build_command : "amd64",
            cmake_path : "",
            gcc_path : "",
            fileReplacement : {
                Destination : dest_dir,
                Files: []
            }
        }

        let subdirs = [
            "dir1", "dir1/dir1", "dir1/dir1/dir3", "dir1/dir2" , "dir2", "dir2/dir1", "dir2/dir2", "dir3"
        ]

        for(const dir of subdirs) {
            await fs.promises.mkdir(path.join(src_dir, dir));
            for(let i = 0; i < 3; i++) {
                let relative_file = path.join(dir, `file${i}`)
                let src_path = path.join(src_dir, relative_file);
                await fs.promises.writeFile(path.join(src_dir, relative_file), crypt.randomBytes(256));
                config.fileReplacement.Files.push(
                    {
                        OnCreatorPath : src_path,
                        RelativeUserPath : relative_file
                    }
                );
            }
        }

        console.log(config);

        await fs.promises.writeFile(path.join(cwd, "sfp_config.json"), JSON.stringify(config), {encoding: 'utf-8'});

        expect(await RunProcesseGetRetCode(configurator_exe, [path.join(cwd, "sfp_config.json")], exe_dir)).toBe(0);

        try{
            await fs.promises.access(path.join(exe_dir, "patche.exe"));
        }catch(err) {
            throw err;
        }

        expect(await RunProcesseGetRetCode(path.join(exe_dir, "patche.exe"), [dest_dir], exe_dir)).toBe(0);

        for(const pth of config.fileReplacement.Files) {
            try{
                await fs.promises.access(path.join(dest_dir, pth.RelativeUserPath))
            } catch(err) {
                throw err;
            }
        }
    })

    afterAll(async () => {
        //await fs.promises.rm(cwd, {recursive: true, force : true})
    });
});