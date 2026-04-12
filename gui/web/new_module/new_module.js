
class New_module{
    constructor(){
        // New module
        this.name = document.getElementById("module_name");
        this.version = document.getElementById("module_version");
        this.author_nick_name = document.getElementById("author_nick_name");
        this.author_full_name = document.getElementById("author_full_name");
        this.description = document.getElementById("module_description");
        this.device_address = document.getElementById("device_address");
        this.comm_selector = document.getElementById("comm-selector");
        this.licence_selector = document.getElementById("licence-selector");
        this.module_location = document.getElementById("module_location");
        this.browse_btn = document.getElementById("btn-new-module-loc-browse");
        this.browse_btn.addEventListener("click" , e => this.state_select("MODULE_BROWSE"));
        // CMake
        this.package_name = document.getElementById("package_name");
        this.linked_libs = document.getElementById("linked_libs");
        // Buttons
        this.cancel_btn = document.getElementById("btn-cancel");
        this.cancel_btn.addEventListener("click" , e => this.state_select("CANCEL")); 
        this.generate_btn = document.getElementById("btn-generate");
        this.generate_btn.addEventListener("click" , e => this.state_select("GENERATE")); 

        this.browse_source = 0;

        console.log("Constructor done!")
    }

    cancel_project_window()
    {
        console.log("Cancel button clicked");
        window.close();
    }

    validate()
    {
        if (!this.name.value || !this.name.value.trim()) {
            alert("Module name is required.");
            this.name.focus();
            return false;
        }
        if (!this.module_location.value || !this.module_location.value.trim()) {
            alert("Module location is required. Use Browse to select a directory.");
            this.module_location.focus();
            return false;
        }
        return true;
    }

    state_select(event)
    {
        console.log("Event received!");
        console.log(event);
        switch(event)
        {
            case "CANCEL": 
                this.cancel_project_window();
                break;
            case "GENERATE":
                if (!this.validate()) break;
                this.generate_btn.textContent = "Generating...";
                this.generate_btn.style.backgroundColor = "";
                eel.generate_module(this.name.value, 
                                    this.version.value, 
                                    this.description.value, 
                                    this.module_location.value, 
                                    this.linked_libs.value,
                                    this.package_name.value,
                                    this.comm_selector.value,
                                    this.device_address.value,
                                    this.author_nick_name.value,
                                    this.author_full_name.value,
                                    this.licence_selector.value
                                    );
                console.log(this);
                break;  
            case "MODULE_BROWSE":
                this.browse_source = "MODULE_BROWSE"
                eel.browse_dir_path();
                break;
        }

    }
}

window.addEventListener('DOMContentLoaded', function() {
    window.nm = new New_module();
});

eel.expose(set_choosen_path_to_input_text);
function set_choosen_path_to_input_text(path)
{
    switch(nm.browse_source)
    {
        case "MODULE_BROWSE":
            nm.module_location.value = path;
            break;
    }
}

eel.expose(successful_generate)
function successful_generate()
{
    nm.generate_btn.style.backgroundColor = "green";
    nm.generate_btn.textContent = "Generate";
    console.log("Successfully generated!");
}

eel.expose(failed_generate)
function failed_generate(error_msg)
{
    nm.generate_btn.style.backgroundColor = "red";
    nm.generate_btn.textContent = "Generate";
    alert("Generation failed: " + error_msg);
    console.error("Generation failed:", error_msg);
}