
class StateComponent extends HTMLElement {
    constructor() {
        super();
        this.attachShadow({ mode: 'open' });
    }

    static get observedAttributes() {
        return ['name', 'level'];
    }

    attributeChangedCallback(name, oldValue, newValue) {
        if (name === 'name') {
            this.name = newValue;
        } else if (name === 'level') {
            this.level = parseInt(newValue);
        }
    }

    connectedCallback() {
        this.render();
    }

    render() {
        const stateName = this.name || 'State';
        const stateLevel = this.level || 0;

        const stateElement = document.createElement('div');
        stateElement.classList.add('state');
        stateElement.style.paddingLeft = `${stateLevel * 20}px`;

        const stateNameElement = document.createElement('div');
        stateNameElement.classList.add('state-name');
        stateNameElement.textContent = stateName;

        stateElement.appendChild(stateNameElement);

        this.shadowRoot.innerHTML = `
            <style>
                .state {
                    display: flex;
                    flex-direction: column;
                }
                .state-name {
                    font-weight: bold;
                }
            </style>
        `;
        this.shadowRoot.appendChild(stateElement);
    }
}

customElements.define('state-component', StateComponent);
