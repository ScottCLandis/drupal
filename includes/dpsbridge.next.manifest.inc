<?php
/* ************************************************************************
 * NOTICE: This work is licensed under a Creative Commons Attribution
 * -Noncommercial-Share Alike 3.0 Unported License. Permissions beyond
 * the scope of this license, pertaining to the examples of code included
 * within this work are available at Adobe Commercial Rights.
 *
 * NOTE: You may reuse these files as you want, but they are not officially
 * supported as part of the product. Technical support will not be able to
 * answer questions about these files.
 *
 * Related Links:
 *  - http://creativecommons.org/licenses/by-nc-sa/3.0/
 *  - http://www.adobe.com/communities/guidelines/ccplus/commercialcode_plus_permission.html
 ************************************************************************* */

/**
 * Custom class that handles HTML article-related contents.
 */
class DPSNextManifest {

    protected $directory;       // path/to/html/article/directory
    protected $zipArchive;      // zip content

    protected $target_viewer;   // metadata target viewer version
    protected $version;         // metadat version number

    protected $index;           // the <index> tag in the manifest
    protected $manifest;        // the <manifest> tag in the manifest
    protected $resources;       // the <resources> tag in the manifest

    /**
     * Manifest class constructor.
     * Set the target viewer version and the version number.
     */
    public function __construct() {
        $this->target_viewer = '33.0.0';
        $this->version = '3.0.0';
    }

    /**
     * Add the path to the HTML article directory.
     * The path must be a directory and valid.
     * All operations will be relative to this path.
     * @param {String} $dir - The path/to/the/HTML/article/directory.
     * @return {Object} $this - The reference to this object class.
     */
    public function addDir($dir) {
        $real_path = realpath($dir);
        if (!file_exists($real_path)) { // invalid path/to/dir
            print_r(__METHOD__, 'the path is invalid: ' . $real_path);
            exit(1);
        } else if (!is_dir($real_path)) { // the specified path is not a dir
            print_r(__METHOD__, 'the path is not a directory: ' . $real_path);
            exit(1);
        } else { // store path/to/dir
            $this->directory = $real_path;
        }
        return $this;
    }

    /**
     * Zip the contents of the HTML article directory.
     * Calls helper to iterate and append files to the zip.
     * @return {Object} $this - The reference to this object class.
     */
    public function buildArticle($destination) {
        // initialize and create zip archive
        $this->zipArchive = new \ZipArchive;
        $this->zipArchive->open($destination, 1);
        // call helper to iterate and append file to zip
        $this->_iterateDir($this->directory, true);
        // close zip
        $this->zipArchive->close();
        return $this;
    }

    /**
     * Generate the manifest XML data.
     * Calls helper to iterate and append files to the zip.
     * @return {Object} $this - The reference to this object class.
     */
    public function generateXML() {
        $this->_initializeXML();
        $this->_iterateDir($this->directory);
        return $this;
    }

    /**
     * Get the manifest data.
     * @return {XML} $manifest - The manifest data.
     */
    public function getXML() {
        return $this->manifest;
    }

    /**
     * Get the path to the generated HTML article zip file.
     * @return {String} $directory - The path/to/HTML/article/zip.
     */
    public function getZipPath() {
        return $this->directory . '/default.article';
    }

    /**
     * Write the manifest XML data to the XML file: manifest.xml.
     * If the file already exists, override it.
     * The ingestion server cannot parse the <xml?> tag, remove it.
     * @return {Object} $this - The reference to this object class.
     */
    public function writeXML() {
        $manifest_file = $this->directory . '/manifest.xml';
        // remove the <xml?\> tag before storing into the XML file
        $manifest_raw = $this->manifest->asXML();
        $manifest_data = substr($manifest_raw, strpos($manifest_raw, '?>') + 3);
        file_put_contents($manifest_file, $manifest_data);
        return $this;
    }

    /**
     * Helper.
     * Initialize the simple XML element object.
     * Store relative references to the <manifest>, <index>, and <resources> tag.
     */
    private function _initializeXML() {
        // generate the manifest XML
        $this->manifest = new \SimpleXMLElement('<manifest/>');
        $this->manifest->addAttribute('dateModified', date('Y-m-d\TH:i:s\Z'));
        $this->manifest->addAttribute('targetViewer', $this->target_viewer);
        $this->manifest->addAttribute('version', $this->version);
        // generate the inner-child: index
        $this->index = $this->manifest->addChild('index');
        // generate the inner-child: resources
        $this->resources = $this->manifest->addChild('resources');
    }

    /**
     * Helper. Recursive.
     * Iterate the given directory of directories.
     * Either read the file data into the manifest or zip file, depending on $is_zip.
     * Only look for the index.html or article.xml within the root directory.
     * @param {String} $dir - The path/to/HTML/article/directory.
     * @param {Boolean} $is_zip - Toggler, to switch to zip function.
     * @param {Boolean} $first_run - Toggler, in root directory if true.
     */
    private function _iterateDir($dir, $is_zip = false, $first_run = true) {
        // get the current list
        $fileIterator = new \DirectoryIterator($dir);
        // iterate the current directory
        foreach ($fileIterator as $file) {
            if ($file->isDot()) { // ignore '.' and '..' directories
                continue;
            } else if ($file->isFile()) { // is file
                if ($is_zip) { // add file to zip
                    $file_fullpath = $file->getPathname();
                    $file_path = $this->_parsePath($file_fullpath);
                    $this->zipArchive->addFile($file_fullpath, $file_path);
                } else { // parse the file
                    $this->_parseFile($file, $is_zip, $first_run);
                }
            } else if ($file->isDir()) { // is folder, recursive
                $sub_dir = $file->getPathname();
                $this->_iterateDir($sub_dir, $is_zip, false);
            }
        }
    }

    /**
     * Helper.
     * Get the root path by removing the path/to/root/directory.
     * @param {String} $file_path - The path/to/given/file.
     * @return {String} $local_path - The path/local/to/root/dir.
     */
    private function _parsePath($file_path) {
        $filtered_path = explode($this->directory . '/', $file_path);
        $local_path = $filtered_path[1];
        return $local_path;
    }

    /**
     * Helper.
     * Get the file information for the manifest.
     * @param {String} $file - The path/to/given/file.
     * @param {Boolean} $is_zip - Toggler, to switch to zip function.
     * @param {Boolean} $first_run - Toggler, in root directory if true.
     */
    private function _parseFile($file, $is_zip, $first_run) {
        $file_name = $file->getFilename();
        $file_ext = $file->getExtension();
        $file_size = $file->getSize();
        $file_fullpath = $file->getPathname();
        // filter the file path (starting from root directory)
        $file_path = $this->_parsePath($file_fullpath);
        // MD5-base64 hash the file content
        $file_content = file_get_contents($file_fullpath);
        $file_md5 = base64_encode(md5($file_content, true));
        // get the respective file type
        switch ($file_ext) {
            case 'css':
                $file_type = 'text/css'; break;
            case 'flv':
                $file_type = 'video/x-flv'; break;
            case 'fvt':
                $file_type = 'video/vnd.fvt'; break;
            case 'f4v':
                $file_type = 'video/x-f4v'; break;
            case 'gif':
                $file_type = 'image/gif'; break;
            case 'html':
                $file_type = 'text/html'; break;
            case 'h261':
                $file_type = 'video/h261'; break;
            case 'h263':
                $file_type = 'video/h263'; break;
            case 'h264':
                $file_type = 'video/h264'; break;
            case 'ico':
                $file_type = 'image/x-icon'; break;
            case 'jpeg':
                $file_type = 'image/jpeg'; break;
            case 'jpg':
                $file_type = 'image/jpeg'; break;
            case 'jpgv':
                $file_type = 'video/jpeg'; break;
            case 'js':
                $file_type = 'application/javascript'; break;
            case 'json':
                $file_type = 'application/json'; break;
            case 'mpeg':
                $file_type = 'video/mpeg'; break;
            case 'mpga':
                $file_type = 'audio/mpeg'; break;
            case 'mp4':
                $file_type = 'video/mp4'; break;
            case 'mp4a':
                $file_type = 'audio/mp4'; break;
            case 'mxml':
                $file_type = 'application/xv+xml'; break;
            case 'm4v':
                $file_type = 'video/x-m4v'; break;
            case 'pdf':
                $file_type = 'application/pdf'; break;
            case 'png':
                $file_type = 'image/png'; break;
            case 'psd':
                $file_type = 'image/vnd.adobe.photoshop'; break;
            case 'svg':
                $file_type = 'image/svg+xml'; break;
            case 'txt':
                $file_type = 'text/plain'; break;
            case 'xml':
                if (!$is_zip && $file_name === 'manifest.xml')
                    return;
                $file_type = 'text/xml';
                break;
            case 'zip':
                error_log(__METHOD__ . '() zip files are not allowed: ' . $file_name, 0);
                return;
            case '':
                error_log(__METHOD__ . '() no file extension: ' . $file_name, 0);
                return;
            default:
                $file_type = 'application/octet-stream'; break;
        }
        // if in root directory and file is either article.xml or index.html,
        // store file as <index> tag within the manifest
        if ($first_run &&
                (strtolower($file_name) === 'article.xml' ||
                 strtolower($file_name) === 'index.html')) {
            $this->index->addAttribute('type', $file_type);
            $this->index->addAttribute('href', $file_path);
        }
        // store file as <resource> tag within the manifest
        $resource = $this->resources->addChild('resource');
        $resource->addAttribute('type', $file_type);
        $resource->addAttribute('href', $file_path);
        $resource->addAttribute('length', $file_size);
        $resource->addAttribute('md5', $file_md5);
    }
}
