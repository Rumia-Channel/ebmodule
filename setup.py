from setuptools import setup, Extension, find_packages
import sys
import os

# Windows 環境かどうかを判定
IS_WINDOWS = sys.platform == 'win32'

# ebmodule の C ソースのパスと、インクルードディレクトリ、ライブラリディレクトリを設定
if IS_WINDOWS:
    EBMODULE_C = 'src\\ebmodule.c'
    INCLUDE_DIRS = ['.\\EBLibrary\\include', '.\\src']
    LIBRARY_DIRS = ['.\\EBLibrary\\lib']
    DLL_PATH = 'EBLibrary/bin/libebu-17.dll'  # 明示的に相対パス指定
else:
    EBMODULE_C = 'src/ebmodule.c'
    INCLUDE_DIRS = ['/usr/local/include', './src']
    LIBRARY_DIRS = None
    DLL_PATH = None

setup(
    name='eb',
    version='2.3',
    description='A wrapper module of the EB library',
    author='Tamito KAJIYAMA',
    author_email='kajiyama@grad.sccs.chukyo-u.ac.jp',
    url='http://pseudo.grad.sccs.chukyo-u.ac.jp/~kajiyama/python/',

    # eb パッケージ内に eblib.py を含める
    packages=find_packages(),
    package_dir={'eb': 'eb'},

    # C拡張モジュール eb を eb パッケージの一部としてインストール
    ext_modules=[
        Extension(
            'eb.eb',  # eb パッケージ内に配置される
            [EBMODULE_C],
            include_dirs=INCLUDE_DIRS,
            library_dirs=LIBRARY_DIRS,
            libraries=['libebu-17']
        )
    ],

    # DLL ファイルを eb フォルダ内にコピー
    data_files=[
        (f'{os.path.join('Lib','site-packages','eb')}', [DLL_PATH])  # DLLをebパッケージ直下にコピー
    ] if IS_WINDOWS else [],

    zip_safe=False,
)
