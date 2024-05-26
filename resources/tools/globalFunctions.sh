xxd_wrapper(){
    all_args="$@"

    has_n=false
    next_arg=""

    while [ "$#" -gt 0 ]; do
        case "$1" in
            -n)
                has_n=true
                next_arg="$2"
                shift 2
                ;;
        esac
            shift
    done

    # Remove -n and its following argument
    all_args="${all_args//-n $next_arg}"

    # We can't save stdin inside a variable because of null bytes - learned it the hard way.
    converted_data=$(cat | xxd $all_args)

    # echo "All arguments: \"$all_args\"" >&2
    # echo "Has -n: $has_n" >&2
    if [ "$has_n" = true ]; then
        # echo "Next argument after -n: $next_arg" >&2
        echo -e -n "const unsigned char $next_arg[] = {\n" 
        echo "$converted_data"
        echo -e "};\nconst size_t ${next_arg}Length = (sizeof($next_arg)/sizeof($next_arg[0]));"
    else
        echo "$converted_data"
    fi
}

get_pio_env() {
    local json_file=$1

    if [ -f "$json_file" ]; then
        # Use jq to extract the projectEnvName from launch.json
        local env_name=$(grep -v '^ *//' "$json_file" | jq '.configurations[0].projectEnvName')
        env_name=${env_name//\"/}
        
        if [ -n "$env_name" ]; then
            echo "$env_name"
        else
            echo "projectEnvName not found in any configuration in $json_file" >&2
            exit
        fi
    else
        echo "File $json_file not found. You are not using vscode. Trying to use env variable" >&2
        if [ -n "$PLATFORMIO_ENV_NAME" ]; then
            echo "PLATFORMIO_ENV_NAME is set with value: $PLATFORMIO_ENV_NAME" >&2
            echo $PLATFORMIO_ENV_NAME
        else
            pio_path="${json_file%%.vscode/*}.pio/"
            first_folder=$(ls ${pio_path}libdeps/ | head -n 1)
            echo "PLATFORMIO_ENV_NAME is not set. Using \"$first_folder\" env" >&2
            echo $first_folder
        fi
    fi
}

extract_serial_port() {
  local esptool_path=$1
  local output
  local last_port

  output=$("$esptool_path" flash_id 2>&1)
  last_port=$(echo "$output" | grep "Serial port" | awk '{print $3}' | tail -n 1)
  echo -n "$last_port"
}