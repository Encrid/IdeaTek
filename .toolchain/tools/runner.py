# Copyright (C) 2026 Enkrid
#
# Under the GNU Affero General Public License, version 3 only: see license.
import sys

from shared import arguments, config, die, this_target

DISPLAY = ['Xvfb :99 -screen 0 1920x1080x24 > /dev/null 2>&1 &',
           "timeout 10 sh -c 'until [ -S /tmp/.X11-unix/X99 ]; do sleep 0.1; done'",
           'DISPLAY=:99 openbox > /dev/null 2>&1 &',
           'mkdir -p -m 700 "$RUNNER_TEMP/wayland"',
           'XDG_RUNTIME_DIR="$RUNNER_TEMP/wayland" weston --backend=headless --renderer=pixman --socket=wayland-1'
           ' --no-config > /dev/null 2>&1 &',
           'timeout 10 sh -c \'until [ -S "$RUNNER_TEMP/wayland/wayland-1" ]; do sleep 0.1; done\'',
           'echo "DISPLAY=:99" >> "$GITHUB_ENV"',
           'echo "WAYLAND_DISPLAY=wayland-1" >> "$GITHUB_ENV"',
           'echo "XDG_RUNTIME_DIR=$RUNNER_TEMP/wayland" >> "$GITHUB_ENV"']
FOLDER = ['%(name)s="$(cygpath -u "$RUNNER_TEMP")/%(name)s"', 'mkdir -p "$%(name)s"']
FETCH = ['curl -fsSL -o "$%(name)s/%(file)s" %(url)s', 'echo "%(digest)s  $%(name)s/%(file)s" | sha256sum -c']
UNPACK = '/c/Windows/System32/tar.exe -xf "$(cygpath -w "$%(name)s/%(file)s")" -C "$(cygpath -w "$%(name)s")"%(members)s'
GL = ['echo "SDL_OPENGL_LIBRARY=$(cygpath -w "$mesa/opengl32.dll")" >> "$GITHUB_ENV"']
VULKAN = ['echo "VK_DRIVER_FILES=$(cygpath -w "$vulkan/%(icd)s")" >> "$GITHUB_ENV"',
          'echo "$(cygpath -w "$vulkan")" >> "$GITHUB_PATH"']
BREW = ['brew install %(packages)s',
        'echo "VK_DRIVER_FILES=$(brew --prefix)/share/vulkan/icd.d/%(icd)s" >> "$GITHUB_ENV"',
        'echo "VULKAN_LOADER=$(brew --prefix)/lib/libvulkan.1.dylib" >> "$GITHUB_ENV"']


def archive(name, file, url, digest, members=''):
    values = {'name': name, 'file': file, 'url': url, 'digest': digest, 'members': members}
    return [line % values for line in FETCH] + [UNPACK % values]


def mesa(runner, driver, cpu, name, file, members=''):
    found = runner['mesa'][driver][cpu]
    url = runner['mesa']['url'].format(version=runner['mesa']['version'], driver=driver, arch=found['arch'])
    return archive(name, file, url, found['sha256'], members)


def windows(needs, cpu, runner):
    lines = []
    if 'gl' in needs:
        lines += [line % {'name': 'mesa'} for line in FOLDER]
        lines += mesa(runner, 'llvmpipe', cpu, 'mesa', 'mesa.7z', ' opengl32.dll') + GL
    if 'vulkan' in needs:
        loader = runner['loader'][cpu]
        url = runner['loader']['url'].format(version=runner['loader']['version'], os=loader['os'], arch=loader['arch'])
        lines += [line % {'name': 'vulkan'} for line in FOLDER]
        lines += mesa(runner, 'lavapipe', cpu, 'vulkan', 'lavapipe.7z')
        lines += archive('vulkan', 'loader.zip', url, loader['sha256'],
                         ' --strip-components=%d %s' % (loader['file'].count('/'), loader['file']))
        lines += [line % {'icd': runner['icd'][cpu]} for line in VULKAN]
    return lines


def commands(needs, machine, runner):
    system, cpu = machine
    if system == 'Linux':
        packages = sorted(package for need in needs for package in runner['packages'][need])
        installed = ['sudo apt-get update', 'sudo apt-get install -y ' + ' '.join(packages)] if packages else []
        return installed + (DISPLAY if 'display' in needs else [])
    if system == 'Windows':
        return windows(needs, cpu, runner)
    if 'vulkan' in needs:
        values = {'packages': ' '.join(runner['brews']['vulkan']), 'icd': runner['icd'][cpu]}
        return [line % values for line in BREW]
    return []


def main():
    args = arguments()
    settings = config(args.root)
    target = args.target or this_target(settings['targets'])
    machine = settings['targets'].get(target, {}).get('runs_on') or die('no machine runs the target %s' % target)
    needs = settings.get('ci', {}).get('needs', [])
    for need in needs:
        if need not in settings['runner']['packages']:
            die('[ci] needs: %s is not a need: %s' % (need, ' '.join(settings['runner']['packages'])))
    sys.stdout.buffer.write(''.join(line + '\n' for line in commands(needs, machine, settings['runner'])).encode('utf-8'))


if __name__ == '__main__':
    main()
